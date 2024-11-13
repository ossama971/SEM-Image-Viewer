#include "ImageFormat.h"

std::string imageFormatToString(ImageFormat imageFormat) {
    switch (imageFormat) {
      case ImageFormat::PNG:
        return "PNG";
      case ImageFormat::JPG:
        return "JPG";
      case ImageFormat::BMP:
        return "BMP";
      default:
        return "Unknown";
    }
}

ImageFormat stringToImageFormat(const std::string &imageFormat) {
  if (boost::iequals(imageFormat, "PNG")) {
    return ImageFormat::PNG;
  } else if (boost::iequals(imageFormat, "JPG")) {
    return ImageFormat::JPG;
  } else if (boost::iequals(imageFormat, "BMP")) {
    return ImageFormat::BMP;
  } else {
    return ImageFormat::Unknown;
  }
}
