#include "ImageRepository.h"

void ImageRepository::load(const std::string &path) {
    _image.load(path);
}

Image& ImageRepository::getImage() {
    return _image;
}
