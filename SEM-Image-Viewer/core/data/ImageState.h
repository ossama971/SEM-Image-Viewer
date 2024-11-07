#ifndef IMAGE_STATE_H
#define IMAGE_STATE_H

#include <opencv2/opencv.hpp>

enum class ImageStateSource
{
    Origin,
    GrayScaleFilter,
    NoiseReductionFilter,
    SharpenFilter,
    EdgeDetectionFilter
};

struct ImageState {
    ImageStateSource State;
    cv::Mat Image;
};

#endif // IMAGE_STATE_H
