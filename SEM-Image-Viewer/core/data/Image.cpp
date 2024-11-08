#include "Image.h"

Image::Image() : _loaded(false) {
}

Image::Image(const Image& image) : _loaded(image._loaded), _path(image._path), _metadata(image._metadata), _states(image._states), _undo(image._undo) {
}

Image::~Image() {
    for (auto it = _states.begin(); it != _states.end(); ++it) {
        it->Image.release();
    }
}

bool Image::load(const std::string &path) {
    _path = path;
    _loaded = true;

    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (!setImage(std::move(image)))
        return false;

    _metadata.load(path, getImageMat());
    return true;
}

bool Image::setImage(cv::Mat image, ImageStateSource newState) {
    if (!_loaded)
        return false;
    if (image.empty())
        return false;

    _states.emplace_front(ImageState{ newState, std::make_unique<cv::Mat>(image) });

    emit onImageStateUpdated(_states);
    return true;
}

Image Image::clone() {
    return Image(*this);
}

cv::Mat& Image::getImageMat() const {
    return *_states.front().Image;
}

ImageStateSource Image::getImageState() const {
    return _states.front().State;
}

std::string Image::getPath() const {
    return _path;
}

ImageMetadata Image::getMetadata() const {
    return _metadata;
}
