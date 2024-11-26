#include "image_state.h"

ImageState::ImageState() {}

ImageState::ImageState(ImageStateSource state, const std::string &imagePath, const std::string &imageExtension)
    : State(state), ImagePath(imagePath), ImageExtension(imageExtension) {}

void ImageState::accept(Visitor &v) const {
    v.visit(*this, nullptr);
}

void ImageState::accept(Visitor &v, Image *parent) const {
    v.visit(*this, parent);
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
