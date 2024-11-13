#ifndef IMAGE_STATE_H
#define IMAGE_STATE_H


#include <string>
#include <boost/algorithm/string.hpp>
#include <opencv2/opencv.hpp>

enum class ImageStateSource {
  Origin,
  GrayScaleFilter,
  NoiseReductionFilter,
  SharpenFilter,
  EdgeDetectionFilter
};

std::string imageStateSourceToString(ImageStateSource state);
ImageStateSource imageStateSourceFromString(const std::string &state);

};

#endif // IMAGE_STATE_H
