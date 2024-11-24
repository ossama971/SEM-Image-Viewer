#ifndef IMAGE_STATE_H
#define IMAGE_STATE_H

#include "Visitable.h"
#include "Visitor.h"

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
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
  std::string ImageExtension;

  ImageState();
  ~ImageState() = default;
  ImageState(ImageStateSource state, cv::Mat image, std::string imageExtension);

  bool save(const std::string &path) const;

  void accept(Visitor &v) const override;
};

#endif // IMAGE_STATE_H
