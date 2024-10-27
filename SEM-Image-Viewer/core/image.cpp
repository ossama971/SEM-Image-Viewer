#include "image.h"

Image::Image(const std::string &path) : path(path) {
  image = cv::imread(path, cv::IMREAD_COLOR);
}

Image::~Image() { image.release(); }

cv::Mat Image::getImage() const { return image; }

std::string Image::getPath() const { return path; }

int Image::getWidth() const { return image.cols; }

int Image::getHeight() const { return image.rows; }

int Image::getSize() const { return image.size().area(); }
