#include "Image.h"

Image::Image() : _loaded(false) {
}

Image::Image(std::filesystem::path path) {
    Image();
    load(path);
}

Image::Image(const Image& image) : _loaded(image._loaded), _path(image._path), _metadata(image._metadata) {
    _states.reserve(image._states.size());
    for (const std::unique_ptr<ImageState>& it : image._states)
        _states.push_back(std::make_unique<ImageState>(*it));

    _redo.reserve(image._redo.size());
    for (const std::unique_ptr<ImageState>& it : image._redo)
        _redo.push_back(std::make_unique<ImageState>(*it));
}

Image::~Image() {
    for (auto it = _states.begin(); it != _states.end(); ++it) {
        (*it)->Image.release();
    }
}

Image Image::operator=(const Image& image) {
    return Image(image);
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
    // if (!_loaded){

    //     return false;
    // }
    if (image.empty()){

        return false;
    }

    _states.push_back(std::make_unique<ImageState>(newState, std::move(image)));

    emit onImageStateUpdated(_states);
    return true;
}

//Image Image::clone() {
//    return Image(*this);
//}

bool  Image::undo(){
    if (!_loaded){
        //logger
        return false;
    }
    if(_states.size()<2){
        //logger
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
        return false;
    }
    if(_redo.empty()){
        //logger
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

std::filesystem::path Image::getPath() const {
    return _path;
}

ImageMetadata Image::getMetadata() const {
    return _metadata;
}
