#ifndef IMAGE_COLOR_SPACE_H
#define IMAGE_COLOR_SPACE_H

#include <boost/algorithm/string.hpp>
#include <string>

enum class ColorSpace { RGB, Gray };

std::string colorSpaceToString(ColorSpace colorSpace);
ColorSpace stringToColorSpace(const std::string &colorSpace);

#endif // IMAGE_COLOR_SPACE_H
