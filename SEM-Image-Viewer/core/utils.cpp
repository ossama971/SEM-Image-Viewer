#include "utils.h"
#include <random>
#include <string>

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
