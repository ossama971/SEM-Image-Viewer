#include "image_cacheless.h"
#include "image_state_cacheless.h"
#include "../utils.h"

ImageCacheless::ImageCacheless() : Image(nullptr) {
}

ImageCacheless::ImageCacheless(std::filesystem::path path) : Image(nullptr, path)
{}

ImageCacheless::ImageCacheless(const ImageCacheless& image) : Image(image)
{}

ImageCacheless::ImageCacheless(ImageCacheless &&image) : Image(image)
{}

ImageCacheless::~ImageCacheless() {
}

bool ImageCacheless::load(const std::filesystem::path &path) {
    _path = path;
    _loaded = true;

    cv::Mat image = cv::imread(path.string(), cv::IMREAD_COLOR);
    if (!setImage(&image))
        return false;

    _metadata.load(getPath().string(), image);
    return true;
}

bool ImageCacheless::setImage(cv::Mat* image, const ImageStateSource newState) {
    if (!image || image->empty())
        return false;

    std::filesystem::path imagePath = getPath();
    std::string imageExtension = imagePath.extension().string();

    _states.push_back(std::make_unique<ImageStateCacheless>(newState, imagePath.string(), *image, imageExtension));

    emit onImageStateUpdated(this);
    return true;
}

void ImageCacheless::addRedo(const cv::Mat &image, const ImageStateSource newState) {
    std::filesystem::path imagePath = getPath();
    std::string imageExtension = imagePath.extension().string();

  _states.push_back(std::make_unique<ImageStateCacheless>(newState, imagePath.string(), std::move(image), imageExtension));
}

const cv::Mat& ImageCacheless::getImageMat() const {
    return static_cast<ImageStateCacheless*>(_states.back().get())->Image;
}

cv::Mat ImageCacheless::readImageMat() const {
    return getImageMat();
}
