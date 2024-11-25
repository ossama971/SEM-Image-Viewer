#include "image_state.h"

ImageState::ImageState() {}

ImageState::ImageState(ImageStateSource state, const cv::Mat &image, const std::string &imageExtension)
    : State(state), Image(image), ImageExtension(imageExtension) {}

void ImageState::accept(Visitor &v) const { v.visit(*this); }

bool ImageState::save(const std::string &path) const {
  return cv::imwrite(path, Image);
}

std::string imageStateSourceToString(ImageStateSource state) {
  switch (state) {
  case ImageStateSource::Origin:
    return "Origin";
  case ImageStateSource::GrayScaleFilter:
    return "GrayScaleFilter";
  case ImageStateSource::NoiseReductionFilter:
    return "NoiseReductionFilter";
  case ImageStateSource::SharpenFilter:
    return "SharpenFilter";
  case ImageStateSource::EdgeDetectionFilter:
    return "EdgeDetectionFilter";
  default:
    return "Unknown";
  }
}

ImageStateSource imageStateSourceFromString(const std::string &state) {
  if (boost::iequals(state, "Origin")) {
    return ImageStateSource::Origin;
  } else if (boost::iequals(state, "GrayScaleFilter")) {
    return ImageStateSource::GrayScaleFilter;
  } else if (boost::iequals(state, "NoiseReductionFilter")) {
    return ImageStateSource::NoiseReductionFilter;
  } else if (boost::iequals(state, "SharpenFilter")) {
    return ImageStateSource::SharpenFilter;
  } else if (boost::iequals(state, "EdgeDetectionFilter")) {
    return ImageStateSource::EdgeDetectionFilter;
  } else {
    return ImageStateSource::Origin;
  }
}
