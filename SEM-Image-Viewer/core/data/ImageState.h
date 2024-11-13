#ifndef IMAGE_STATE_H
#define IMAGE_STATE_H

#include "Visitable.h"
#include "Visitor.h"

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

struct ImageState : public Visitable {
  ImageStateSource State;
  cv::Mat Image;

  ImageState();
  ImageState(ImageStateSource state, cv::Mat image);

  void accept(Visitor &v) const override;
};

#endif // IMAGE_STATE_H
