#include "utils.h"
#include <random>
#include <string>

#include "./data/image.h"
#include "./data/image_repository.h"
#include "./data/image_state.h"
#include "./data/session_data.h"
#include "./engines/logger.h"
#include "./engines/workspace.h"
#include "./engines/thread_pool.h"
#include "./engines/logger.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

cv::Mat Utils::diffTwoImages(const cv::Mat &image1, const cv::Mat &image2,
                             const int threshold)
{
  cv::Mat diff;
  cv::Mat image_one = image1.clone();
  cv::Mat image_two = image2.clone();

  // Ensure both images are of the same type
  if (image1.channels() == 1 && image2.channels() != 1)
  {
    cv::cvtColor(image2, image_two, cv::COLOR_BGR2GRAY);
  }
  if (image1.channels() != 1 && image2.channels() == 1)
  {
    cv::cvtColor(image1, image_one, cv::COLOR_BGR2GRAY);
  }

  cv::absdiff(image_one, image_two, diff);
  cv::Mat mask(diff.rows, diff.cols, CV_8UC1, cv::Scalar(0));

  if (diff.channels() == 1)
  { // Grayscale images
    for (int j = 0; j < diff.rows; ++j)
    {
      for (int i = 0; i < diff.cols; ++i)
      {
        int val = diff.at<unsigned char>(j, i);
        if (val > threshold)
        {
          mask.at<unsigned char>(j, i) = 255;
        }
      }
    }
  }
  else if (diff.channels() == 3)
  { // Color images
    for (int j = 0; j < diff.rows; ++j)
    {
      for (int i = 0; i < diff.cols; ++i)
      {
        cv::Vec3b pix = diff.at<cv::Vec3b>(j, i);
        int val = pix[0] + pix[1] + pix[2];
        if (val > threshold)
        {
          mask.at<unsigned char>(j, i) = 255;
        }
      }
    }
  }

  cv::Mat res;
  cv::bitwise_and(image_one, image_one, res, mask);
  Logger::instance()->logMessage(
      Logger::MessageTypes::info, Logger::MessageID::differerence_images,
      Logger::MessageOption::without_path,
      {});
  return res;
}

std::string Utils::generateString(size_t length)
{
  std::string out;
  out.reserve(length);
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

  for (size_t i = 0; i < length; ++i)
  {
    char c = alphanum[dis(gen)];
    out.push_back(c);
  }
  return out;
}

cv::Mat Utils::heatmap(const cv::Mat &image)
{
  cv::Mat gray = image;
  if (image.channels() != 1)
  {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  }
  cv::applyColorMap(gray, gray, cv::COLORMAP_JET);
  return gray;
}

bool Utils::createDirectory(const std::string &path)
{
  if (std::filesystem::exists(path))
  {
    return true;
  }
  else
  {
    // create the directory
    // qDebug() << "Creating directory: " << path.c_str();
    if (std::filesystem::create_directory(path))
    {
      return true;
    }
    else
    {
      // Logger::instance()->log(std::make_unique<ErrorMessage>(
      //     1, boost::format("Error creating directory: %1%") % path));
      return false;
    }
  }
}

void Utils::loadSessionJson(const std::string &filename)
{
  boost::property_tree::ptree root;
  try
  {
    boost::property_tree::read_json(filename, root);
  }
  catch (const boost::property_tree::json_parser_error &e)
  {
    // Handle JSON parsing error
    return;
  }

  try
  {
    const auto &session_data = root.get_child("SessionData");
    const auto &repo_data = session_data.get_child("ImageRepository");

    std::string folderPath = repo_data.get<std::string>("_folderPath");
    Workspace::Instance()->getActiveSession().loadDirectory(folderPath);

    const auto &images = repo_data.get_child("Images");
    const std::size_t total_images = images.size();
    const std::size_t batch_size = 17;

    std::vector<boost::property_tree::ptree> image_nodes;
    for (const auto &image_node : images)
    {
      image_nodes.push_back(image_node.second);
    }

    int progressbarID = Logger::instance()->logMessageWithProgressBar(
        Logger::MessageTypes::info,
        Logger::MessageID::loading_session,
        Logger::MessageOption::without_path,
        {QString::fromStdString(filename)},
        total_images

    );

    std::vector<std::future<void>> futures;
    std::atomic<size_t> processed_images{0};

    for (std::size_t i = 0; i < total_images; i += batch_size)
    {
      auto start = image_nodes.begin() + i;
      auto end = (i + batch_size < total_images) ? start + batch_size : image_nodes.end();
      std::vector<boost::property_tree::ptree> batch(start, end);

      futures.push_back(post(ThreadPool::instance(), use_future([progressbarID,
                                                                 folderPath,
                                                                 batch,
                                                                 &processed_images,
                                                                 total_images]()
                                                                {
        for (const auto &node : batch) {
          try {
            std::string imagePath = node.get<std::string>("_path");
            bool isLoaded = node.get<bool>("_loaded");

            Image *image =
                Workspace::Instance()->getActiveSession().getImageRepo().getImage(imagePath);
            if (!image) {
              // Log missing image
              continue;
            }

            for (const auto &state_node : node.get_child("states")) {
              std::string state = state_node.second.get<std::string>("state");
              std::string imageFile = state_node.second.get<std::string>("image");
              cv::Mat imageMat = cv::imread(imageFile);
              image->setImage(&imageMat,
                              imageStateSourceFromString(state));
            }

            for (const auto &undo_node : node.get_child("undo")) {
              std::string state = undo_node.second.get<std::string>("state");
              std::string imageFile = undo_node.second.get<std::string>("image");
              image->addRedo(cv::imread(imageFile),
                             imageStateSourceFromString(state));
            }
          } catch (const boost::property_tree::ptree_error &e) {
            // Handle parsing errors for individual nodes
          }

          processed_images++;
          float progress = static_cast<float>(processed_images.load()) / total_images;
          Logger::instance()->updateProgressBar(progressbarID, progress);
        } })));
    }

    for (auto &future : futures)
    {
      future.get();
    }
  }
  catch (const boost::property_tree::ptree_error &e)
  {
    // Handle other JSON structure errors
  }
}

