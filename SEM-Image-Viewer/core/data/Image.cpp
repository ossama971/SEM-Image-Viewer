#include "Image.h"
#include <QDebug>
#include "../engines/Logger.h"


Image::Image() : _loaded(false) {
}

Image::Image(std::filesystem::path path) { load(path); }

Image::Image(const Image& image) : _loaded(image._loaded), _path(image._path), _metadata(image._metadata) {
    _states.reserve(image._states.size());
    for (const std::unique_ptr<ImageState>& it : image._states)
        _states.push_back(std::make_unique<ImageState>(*it));

    _redo.reserve(image._redo.size());
    for (const std::unique_ptr<ImageState>& it : image._redo)
        _redo.push_back(std::make_unique<ImageState>(*it));
}

Image::Image(Image &&image)
    : _loaded(image._loaded), _path(std::move(image._path)),
      _metadata(std::move(image._metadata)), _states(std::move(image._states)),
      _redo(std::move(image._redo)) {
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

  _redo.clear();
  for (const auto &undoState : image._redo) {
    _redo.push_back(std::make_unique<ImageState>(*undoState));
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
  _redo = std::move(image._redo);

  image._loaded = false;

  return *this;
}

Image::~Image() {
  for (auto it = _states.begin(); it != _states.end(); ++it) {
    (*it)->Image.release();
  }
  for (auto it = _redo.begin(); it != _redo.end(); ++it) {
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
    if (image.empty()){
        return false;
    }
  std::string imageExtension = this->_path.extension().string();
  _states.push_back(std::make_unique<ImageState>(newState, std::move(image), imageExtension));
  emit onImageStateUpdated(_states);
  return true;
}

void Image::addRedo(cv::Mat image, ImageStateSource newState) {
  std::string imageExtension = this->_path.extension().string();
  _states.push_back(std::make_unique<ImageState>(newState, std::move(image), imageExtension));
}

bool Image::isLoaded() const { return _loaded; }

bool  Image::undo(){
    if (!_loaded){
       // Logger::instance()->log(std::make_unique<ErrorMessage>(LOG_ERROR,boost::format(LogMessageMapper::generalError("Undoing the Actions").toStdString())));
        return false;
    }
    if(_states.size()<2){
        //logger
       // Logger::instance()->log(std::make_unique<ErrorMessage>(LOG_ERROR,boost::format(LogMessageMapper::generalError("Undoing the Actions").toStdString())));
        return false;
    }

    _redo.push_back(std::move(_states.back()));

    _states.pop_back();

    emit onImageStateUpdated(_states);
    return true;
}

bool  Image::redo(){
    if (!_loaded){
        //logger
//Logger::instance()->log(std::make_unique<ErrorMessage>(LOG_ERROR,boost::format(LogMessageMapper::generalError("Redoing the Actions").toStdString())));
        return false;
    }
    if(_redo.empty()){
        //logger
        // Logger::instance()->log(std::make_unique<ErrorMessage>(LOG_ERROR,boost::format(LogMessageMapper::generalError("Redoing the Actions").toStdString())));
        return false;
    }

    _states.push_back(std::move(_redo.back()));

    _redo.pop_back();

    emit onImageStateUpdated(_states);
    return true;
}

QList<QString> Image::getHistory(){
    QList<QString> actionsList;

    for(int i=0;i<_states.size();i++){
        if(_states[i]->State==ImageStateSource::GrayScaleFilter){
            actionsList.append("Gray Scale Filter");
        }
        else if(_states[i]->State==ImageStateSource::NoiseReductionFilter){
            actionsList.append("Noise Reduction Filter");
        }
        else if(_states[i]->State==ImageStateSource::SharpenFilter){
            actionsList.append("Sharpen Filter");
        }
        else if(_states[i]->State==ImageStateSource::EdgeDetectionFilter){
            actionsList.append("Edge Detection Filter");
        }
    }
    return actionsList;
}

cv::Mat& Image::getImageMat() const {
    // qDebug() << "num of states in the image: "<<_states.size()<<" alooo";
    return _states.back()->Image;
}

ImageStateSource Image::getImageState() const {
    return _states.back()->State;
}

QString Image::GetCurrentAction() const {
    if(_states.back()->State==ImageStateSource::GrayScaleFilter){
        return "Gray Scale Filter";
    }
    else if(_states.back()->State==ImageStateSource::NoiseReductionFilter){
         return  "Noise Reduction Filter";
    }
    else if(_states.back()->State==ImageStateSource::SharpenFilter){
        return "Sharpen Filter";
    }
    else if(_states.back()->State==ImageStateSource::EdgeDetectionFilter){
        return "Edge Detection Filter";
    }
    return "";
}

std::filesystem::path Image::getPath() const { return _path; }

std::vector<std::unique_ptr<ImageState>> const &Image::getStates() const {
  return _states;
}

std::vector<std::unique_ptr<ImageState>> const &Image::getUndo() const {
  return _redo;
}

ImageMetadata Image::getMetadata() const { return _metadata; }

void Image::accept(Visitor &v) const { v.visit(*this); }
