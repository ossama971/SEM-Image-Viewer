#include "ImageState.h"

ImageState::ImageState() {}

ImageState::ImageState(ImageStateSource state, cv::Mat image)
    : State(state), Image(image) {}

void ImageState::accept(Visitor &v) const { v.visit(*this); }

// TODO: I think images states should be stored as files in the file system
// after being compressed into jpeg, since jpeg is already less space than
// base64. This way saving & loading the states should be more efficient
std::string ImageState::getImageBase64() const {
  std::vector<uchar> buf;
  cv::imencode(".jpeg", Image, buf);
  std::string base64_string;
  using namespace boost::archive::iterators;
  typedef base64_from_binary<transform_width<uchar*, 6, 8>> base64_enc;
  base64_string = std::string(base64_enc(buf.data()), base64_enc(buf.data() + buf.size()));
  return base64_string;
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
