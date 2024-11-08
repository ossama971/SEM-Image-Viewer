#include "Image.h"

Image::Image() : _loaded(false) {
}

Image::Image(const std::string &path) {
    Image();
    load(path);
}

Image::Image(const Image& image) : _loaded(image._loaded), _path(image._path), _metadata(image._metadata)
    //, _states(image._states), _undo(image._undo)
{
    _states.reserve(image._states.size());
    for (const std::unique_ptr<ImageState>& it : image._states)
        _states.push_back(std::make_unique<ImageState>(*it));

    _undo.reserve(image._undo.size());
    for (const std::unique_ptr<ImageState>& it : image._undo)
        _undo.push_back(std::make_unique<ImageState>(*it));
}

Image::~Image() {
    for (auto it = _states.begin(); it != _states.end(); ++it) {
        (*it)->Image.release();
    }
}

Image Image::operator=(const Image& image) {
    return Image(image);
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

    _states.push_back(std::make_unique<ImageState>(newState, std::move(image)));

    emit onImageStateUpdated(_states);
    return true;
}

//Image Image::clone() {
//    return Image(*this);
//}

cv::Mat& Image::getImageMat() const {
    return _states.front()->Image;
}

ImageStateSource Image::getImageState() const {
    return _states.front()->State;
}

std::string Image::getPath() const {
    return _path;
}

ImageMetadata Image::getMetadata() const {
    return _metadata;
}
