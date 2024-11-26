#ifndef IMAGE_STATE_CACHLESS_H
#define IMAGE_STATE_CACHLESS_H

#include "image_state.h"

struct ImageStateCacheless : public ImageState {
    cv::Mat Image;

    ImageStateCacheless();
    ImageStateCacheless(const ImageStateSource state, const std::string &imagePath, const cv::Mat &image, const std::string &imageExtension);
};

#endif // IMAGE_STATE_CACHLESS_H
