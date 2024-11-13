#include "Image.h"
#include <QDebug>

Image::Image() : _loaded(false) {}

Image::Image(std::filesystem::path path) { load(path); }

Image::Image(const Image &image)
    : _loaded(image._loaded), _path(image._path), _metadata(image._metadata) {
  _states.reserve(image._states.size());
  for (const std::unique_ptr<ImageState> &it : image._states)
    _states.push_back(std::make_unique<ImageState>(*it));

  _undo.reserve(image._undo.size());
  for (const std::unique_ptr<ImageState> &it : image._undo)
    _undo.push_back(std::make_unique<ImageState>(*it));
}

Image::Image(Image &&image)
    : _loaded(image._loaded), _path(std::move(image._path)),
      _metadata(std::move(image._metadata)), _states(std::move(image._states)),
      _undo(std::move(image._undo)) {
  image._loaded = false;
}

Image &Image::operator=(const Image &image) {
  if (this == &image)
    return *this;

  _loaded = image._loaded;
  _path = image._path;
  _metadata = image._metadata;

  _states.clear();
  for (const auto &state : image._states) {
    _states.push_back(std::make_unique<ImageState>(*state));
  }

  _undo.clear();
  for (const auto &undoState : image._undo) {
    _undo.push_back(std::make_unique<ImageState>(*undoState));
  }

  return *this;
}

Image &Image::operator=(Image &&image) {
  if (this == &image)
    return *this;

  _loaded = image._loaded;
  _path = std::move(image._path);
  _metadata = std::move(image._metadata);
  _states = std::move(image._states);
  _undo = std::move(image._undo);

  image._loaded = false;

  return *this;
}

Image::~Image() {
  for (auto it = _states.begin(); it != _states.end(); ++it) {
    (*it)->Image.release();
  }
}

bool Image::load(const std::filesystem::path path) {
  _path = path;
  _loaded = true;

  cv::Mat image = cv::imread(path.string(), cv::IMREAD_COLOR);
  if (!setImage(std::move(image)))
    return false;

  _metadata.load(path.string(), getImageMat());
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

bool Image::isLoaded() const { return _loaded; }

cv::Mat &Image::getImageMat() const { return _states.front()->Image; }

ImageStateSource Image::getImageState() const { return _states.front()->State; }

std::filesystem::path Image::getPath() const { return _path; }

std::vector<std::unique_ptr<ImageState>> const &Image::getStates() const {
  return _states;
}

std::vector<std::unique_ptr<ImageState>> const &Image::getUndo() const {
  return _undo;
}

ImageMetadata Image::getMetadata() const { return _metadata; }

void Image::accept(Visitor &v) const { v.visit(*this); }
