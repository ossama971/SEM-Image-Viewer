#include "ImageColorSpace.h"

std::string colorSpaceToString(ColorSpace colorSpace) {
    switch (colorSpace) {
        case ColorSpace::RGB:
            return "RGB";
        case ColorSpace::Gray:
            return "Gray";
        default:
            return "Unknown";
    }
}

ColorSpace stringToColorSpace(const std::string &colorSpace) {
  if (boost::iequals(colorSpace, "RGB")) {
    return ColorSpace::RGB;
  } else if (boost::iequals(colorSpace, "Gray")) {
    return ColorSpace::Gray;
  } else {
    return ColorSpace::Gray;
  }
}
