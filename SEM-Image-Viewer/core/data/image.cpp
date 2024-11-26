#include "image.h"
#include "image_cache_pool.h"
#include "../utils.h"

Image::Image(ImageCachePool* cachePool) : _loaded(false), _cachePool(cachePool) {
}

Image::Image(ImageCachePool* cachePool, std::filesystem::path path) : Image(cachePool) {
    load(path);
}

Image::Image(const Image& image) : _loaded(image._loaded), _path(image._path), _metadata(image._metadata), _cachePool(image._cachePool) {
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
      _redo(std::move(image._redo)), _cachePool(image._cachePool) {
  image._loaded = false;
}

Image &Image::operator=(const Image &image) {
  if (this == &image)
    return *this;

  _loaded = image._loaded;
  _path = image._path;
  _metadata = image._metadata;
  _cachePool = image._cachePool;

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

    _loaded = false;
    _path = std::move(image._path);
    _metadata = std::move(image._metadata);
    _cachePool = image._cachePool;
    _states = std::move(image._states);
    _redo = std::move(image._redo);

    return *this;
}

Image::~Image() {
}

bool Image::load(const std::filesystem::path &path) {
    _path = path;
    _loaded = true;

    if (!setImage(nullptr))
        return false;

    return true;
}

bool Image::save(const std::string &path, ImageState *state) {
    if (!state)
        return false;

    cv::Mat* imagePtr = _cachePool->get(state->ImagePath, false);
    if (imagePtr)
        return save(path, state, *imagePtr);

    cv::Mat image;
    if (std::filesystem::exists(state->ImagePath))
        image = cv::imread(state->ImagePath);
    return save(path, state, image);
}

bool Image::setImage(cv::Mat *image, const ImageStateSource newState) {
    if (image && image->empty())
        return false;

    std::filesystem::path imagePath = getPath(newState);
    std::string imageExtension = _path.extension().string();

    _states.push_back(std::make_unique<ImageState>(newState, imagePath.string(), imageExtension));

    if (image)
        _cachePool->set(imagePath.string(), image);

    emit onImageStateUpdated(this);
    return true;
}

void Image::addRedo(const cv::Mat &image, const ImageStateSource newState) {
    std::filesystem::path imagePath = getPath(newState);
    std::string imageExtension = _path.extension().string();

    _cachePool->set(imagePath.string(), (cv::Mat*)&image);

    _states.push_back(std::make_unique<ImageState>(newState, imagePath.string(), imageExtension));
}

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

    emit onImageStateUpdated(this);
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

    emit onImageStateUpdated(this);
    return true;
}

void Image::accept(Visitor &v) const {
    v.visit(*this);
}

void Image::onCacheImageLoaded(const std::string &path, cv::Mat *image) {
    if (!image || _states.empty() || _states.back()->ImagePath.compare(path))
        return;

    _metadata.load(path, *image);
    emit onImageStateUpdated(this);

#ifdef GRID_VIEW_MANUAL_UPDATE
    emit onCacheLoaded();
#endif
}

bool Image::save(const std::string &path, ImageState *state, const cv::Mat &image) {
    if (!state || image.empty())
        return false;

    return cv::imwrite(path, image);
}

cv::Mat* Image::getImageMat(bool autoLoad) const {
    const std::string& path = _states.back()->ImagePath;
    cv::Mat* image = _cachePool->get(path, autoLoad);

    if (image != nullptr)
        return image;

    return autoLoad ? _cachePool->getImageLoadingTemplate() : nullptr;
}

std::filesystem::path Image::getPath(const ImageStateSource newState) const {
    return newState == ImageStateSource::Origin ?
               _path :
               std::filesystem::path(
                   std::filesystem::current_path().string() + "/" + IMAGE_CACHE_DIR "/"
                   + _path.filename().replace_extension().string() + "_"
                   + imageStateSourceToString(newState) + "_" + Utils::generateString(8)
                   + _path.extension().string()
                   );
}

bool Image::isLoaded() const {
    return _loaded;
}

const cv::Mat& Image::getImageMat() const {
    return *getImageMat(true);
}

cv::Mat Image::readImageMat() const {
    cv::Mat* imagePtr = getImageMat(false);
    if (imagePtr)
        return *imagePtr;

    cv::Mat image;
    const std::string& imagePath = _states.back()->ImagePath;

    if (std::filesystem::exists(imagePath))
        image = cv::imread(imagePath);
    return image;
}

const QList<QString> Image::getHistory(){
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

ImageStateSource Image::getImageState() const {
    return _states.back()->State;
}

const QString Image::getCurrentAction() const {
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

std::filesystem::path Image::getPath() const {
    return _path;
}

std::vector<std::unique_ptr<ImageState>> const &Image::getStates() const {
    return _states;
}

std::vector<std::unique_ptr<ImageState>> const &Image::getUndo() const {
    return _redo;
}

ImageMetadata Image::getMetadata() const {
    return _metadata;
}
