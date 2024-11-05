#include "Image.h"

void Image::load(const std::string &path) {
    _path = path;
    _image = cv::imread(path, cv::IMREAD_COLOR);
}

Image::~Image() {
    _image.release();
}

cv::Mat Image::getImageMat() const {
    return _image;
}

std::string Image::getPath() const {
    return _path;
}

ImageMetadata Image::getMetadata() const {
    return _metadata;
}

void Image::setMat(cv::Mat _image){
    this->_image=_image;
}
