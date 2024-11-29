#include "utils.h"
#include <QFile>
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

bool Utils::checkWritePermission(const std::filesystem::path &folderPath) {
  try {
    std::filesystem::path tempFilePath = folderPath / ".write_test";
    std::ofstream tempFile(tempFilePath.string());
    if (!tempFile.is_open()) {
      throw std::ios_base::failure("Unable to create file in the selected directory.");
    }
    tempFile.close();
    std::filesystem::remove(tempFilePath);
    return true;
  } catch (const std::exception &e) {
    return false;
  }
}

bool Utils::createDirectory(const std::string &path) {
  if (std::filesystem::exists(path)) {
    return true;
  }
  else
  {
    if (std::filesystem::create_directory(path)) {
      return true;
    }
    else {
      return false;
    }
  }
}

bool Utils::checkReadPermission(const std::filesystem::path &path) {
  if (!std::filesystem::exists(path)) {
        return false;
    }
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }
    return true;
}

void Utils::loadSessionJson(const std::string &filename) {
  if(!checkReadPermission(filename)) {
    Logger::instance()->logMessage(
        Logger::MessageTypes::error,
        Logger::MessageID::insufficient_permissions,
        Logger::MessageOption::without_path,
        {QString::fromStdString(filename)});
    return;
  }

  if (!std::filesystem::exists(filename)) {
    return;
  }

  boost::property_tree::ptree root;
  try {
    boost::property_tree::read_json(filename, root);
  }
  catch (const boost::property_tree::json_parser_error &e) {
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

    Workspace::Instance()->getActiveSession().getImageRepo().setHasUnsavedChanges(false);
  }
  catch (const boost::property_tree::ptree_error &e)
  {
    // Handle other JSON structure errors
  }
}

QImage Utils::loadFromQrc(const QString &qrc, const char *extension) {
    QFile file(qrc);
    QImage img;

    if(file.open(QIODevice::ReadOnly))
        img.load(&file, extension);

    return img;
}

cv::Mat Utils::loadFromQrc(const QString &qrc, int flag) {
    QFile file(qrc);
    cv::Mat img;

    if(file.open(QIODevice::ReadOnly))
    {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        img = cv::imdecode(buf, flag);
    }
    return img;
}

cv::Mat Utils::imageToMat(const QImage &image) {
    if (image.isNull())
        return cv::Mat();

    switch (image.format())
    {
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::Mat result;

        cv::cvtColor(mat, result, cv::COLOR_BGRA2BGR);
        return result;
    }

    case QImage::Format_RGB888:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    case QImage::Format_Indexed8:
    {
        if (!image.colorTable().isEmpty())
        {
            // Convert to a color-mapped image (indexed colors with a palette)
            QImage colorMappedImage = image.convertToFormat(QImage::Format_RGB888);
            cv::Mat mat(colorMappedImage.height(), colorMappedImage.width(), CV_8UC3, (void*)colorMappedImage.constBits(), colorMappedImage.bytesPerLine());
            return mat.clone();  // Return a deep copy of the color-mapped image
        }

        // If no color table, treat as grayscale and convert to CV_8UC1
        cv::Mat mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    case QImage::Format_RGB16:
    {
        cv::Mat mat(image.height(), image.width(), CV_16UC3, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    case QImage::Format_ARGB8565_Premultiplied:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    case QImage::Format_Grayscale8:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    default:
    {
        // If we encounter any other format, convert to ARGB32 and proceed
        QImage converted = image.convertToFormat(QImage::Format_ARGB32);
        cv::Mat mat(converted.height(), converted.width(), CV_8UC4, (void*)converted.constBits(), converted.bytesPerLine());
        return mat.clone();
    }
    }
}

QImage Utils::matToImage(const cv::Mat &image) {
    if (image.empty())
        return QImage();

    // Convert from CV_8UC3 (BGR format) to QImage::Format_RGB888
    if (image.channels() == 3)
    {
        QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }

    // Convert from CV_8UC1 (grayscale) to QImage::Format_Indexed8
    else if (image.channels() == 1)
        return QImage(image.data, image.cols, image.rows, image.step, QImage::Format_Indexed8);

    // Convert from CV_8UC4 (BGRA format) to QImage::Format_ARGB32
    else if (image.channels() == 4)
        return QImage(image.data, image.cols, image.rows, image.step, QImage::Format_ARGB32);

    // If the input cv::Mat is an unsupported format
    return QImage();
}
