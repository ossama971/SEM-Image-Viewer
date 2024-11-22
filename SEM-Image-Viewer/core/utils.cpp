#include "utils.h"
#include <memory>
#include <random>
#include <string>

#include "./data/ErrorMessage.h"
#include "./data/Image.h"
#include "./data/ImageRepository.h"
#include "./data/ImageState.h"
#include "./data/SessionData.h"
#include "./engines/Logger.h"
#include "./engines/Workspace.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

cv::Mat Utils::diffTwoImages(const cv::Mat &image1, const cv::Mat &image2,
                             const int threshold) {
  cv::Mat diff;
  cv::absdiff(image1, image2, diff);
  cv::Mat mask(diff.rows, diff.cols, CV_8UC1, cv::Scalar(0));
  for (int j = 0; j < diff.rows; ++j) {
    for (int i = 0; i < diff.cols; ++i) {
      cv::Vec3b pix = diff.at<cv::Vec3b>(j, i);
      int val = (pix[0] + pix[1] + pix[2]);
      if (val > threshold) {
        mask.at<unsigned char>(j, i) = 255;
      }
    }
  }
  cv::Mat res;
  cv::bitwise_and(image1, image1, res, mask);
  return res;
}

std::string Utils::generateString(size_t length) {
  std::string out;
  out.reserve(length);
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

  for (size_t i = 0; i < length; ++i) {
    char c = alphanum[dis(gen)];
    out.push_back(c);
  }
  return out;
}

bool Utils::createDirectory(const std::string &path) {
  if (std::filesystem::exists(path)) {
    return true;
  } else {
    // create the directory
    qDebug() << "Creating directory: " << path;
    if (std::filesystem::create_directory(path)) {
      return true;
    } else {
      Logger::instance()->log(std::make_unique<ErrorMessage>(
          1, boost::format("Error creating directory: %1%") % path));
      return false;
    }
  }
}

void Utils::loadSessionJson(const std::string &filename) {
  boost::property_tree::ptree root;
  try {
    boost::property_tree::read_json(filename, root);
  } catch (const boost::property_tree::json_parser_error &e) {
    Logger::instance()->log(
        std::make_unique<ErrorMessage>(1, boost::format("Sharpening image")));
    return;
  }

  try {
    const auto &session_data = root.get_child("SessionData");
    const auto &repo_data = session_data.get_child("ImageRepository");

    std::string folderPath = repo_data.get<std::string>("_folderPath");
    // load the previously loaded directory with all the images inside it
    // this call loads from the directory into memory
    Workspace::Instance()->getActiveSession().loadDirectory(folderPath);

    for (const auto &image_node : repo_data.get_child("Images")) {
      std::string imagePath = image_node.second.get<std::string>("_path");
      bool isLoaded = image_node.second.get<bool>("_loaded");

      Image *image =
          Workspace::Instance()->getActiveSession().getImageRepo().getImage(
              imagePath);
      if (image == nullptr) {
        Logger::instance()->log(std::make_unique<ErrorMessage>(
            1, boost::format(
                   "Image(%1%) not found in the ImageRepo(%2%) anymore") %
                   imagePath % folderPath));
        continue;
      }

      for (const auto &state_node : image_node.second.get_child("states")) {
        std::string state = state_node.second.get<std::string>("state");
        std::string imageFile = state_node.second.get<std::string>("image");
        image->setImage(cv::imread(imageFile),
                        imageStateSourceFromString(state));
      }

      for (const auto &undo_node : image_node.second.get_child("undo")) {
        std::string state = undo_node.second.get<std::string>("state");
        std::string imageFile = undo_node.second.get<std::string>("image");
        image->addRedo(cv::imread(imageFile),
                       imageStateSourceFromString(state));
      }
    }
  } catch (const boost::property_tree::ptree_error &e) {
    Logger::instance()->log(std::make_unique<ErrorMessage>(
        1, boost::format("Error parsing JSON: %1%") % e.what()));
  }
}

std::optional<std::pair<QImage, QString>> Utils::prepareImageForExport(
    const Image *image, const QString &directoryPath, const QString &format) {
  if (!image) {
    qDebug() << "Invalid image.";
    return std::nullopt;
  }

  // Extract base name
  std::string fileName = image->getPath().filename().string();
  size_t lastDot = fileName.find_last_of('.');
  if (lastDot != std::string::npos) {
    fileName = fileName.substr(0, lastDot); // Remove the extension
  }
  QString baseName = QString::fromStdString(fileName);

  // Convert cv::Mat to QImage
  cv::Mat matImg = image->getImageMat();
  QImage qImg;
  if (matImg.channels() == 3) {
    qImg = QImage(matImg.data, matImg.cols, matImg.rows, matImg.step[0],
                  QImage::Format_RGB888)
               .rgbSwapped();
  } else if (matImg.channels() == 1) {
    qImg = QImage(matImg.data, matImg.cols, matImg.rows, matImg.step[0],
                  QImage::Format_Grayscale8);
  } else {
    qDebug() << "Unsupported image format.";
    return std::nullopt;
  }

  // Construct full file path
  QString numberedFileName =
      QString("%1/%2.%3").arg(directoryPath, baseName, format);

  return std::make_pair(qImg, numberedFileName);
}
