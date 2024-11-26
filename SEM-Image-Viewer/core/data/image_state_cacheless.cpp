#include "image_state_cacheless.h"

ImageStateCacheless::ImageStateCacheless() {}

ImageStateCacheless::ImageStateCacheless(const ImageStateSource state, const std::string &imagePath, const cv::Mat &image, const std::string &imageExtension)
    : ImageState(state, imagePath, imageExtension), Image(image)
{}
