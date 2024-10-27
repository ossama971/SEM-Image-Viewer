#include "image_utils.h"

cv::Mat ImageUtils::getImageProperties(const std::string &path) {
  cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
  return image;
}

std::vector<int>
ImageUtils::pixelIntensity(const cv::Mat &image,
                           const std::vector<std::pair<int, int>> &points) {
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  std::vector<int> intensities;
  for (const auto &point : points) {
    intensities.push_back(gray.at<uchar>(point.first, point.second));
  }
  return intensities;
}

cv::Mat ImageUtils::heatMap(const cv::Mat &image) {
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  cv::applyColorMap(gray, gray, cv::COLORMAP_JET);
  return gray;
}

cv::Mat ImageUtils::diffTwoImages(const cv::Mat &image1, const cv::Mat &image2,
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
