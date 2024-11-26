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

std::string imageStateSourceToString(const ImageStateSource state);
ImageStateSource imageStateSourceFromString(const std::string &state);

struct ImageState : public Visitable {
  ImageStateSource State;
  std::string ImagePath;
  std::string ImageExtension;

  ImageState();
  ~ImageState() = default;
  ImageState(const ImageStateSource state, const std::string &imagePath, const std::string &imageExtension);

  void accept(Visitor &v) const override;
  void accept(Visitor &v, class Image *parent) const;
};

#endif // IMAGE_STATE_H
