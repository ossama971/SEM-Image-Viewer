#ifndef IMAGE_FORMAT_H
#define IMAGE_FORMAT_H

#include <boost/algorithm/string.hpp>
#include <string>

enum class ImageFormat { Unknown, PNG, JPG, BMP };

std::string imageFormatToString(ImageFormat imageFormat);
ImageFormat stringToImageFormat(const std::string &imageFormat);

#endif // IMAGE_FORMAT_H
