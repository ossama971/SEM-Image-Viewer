#ifndef IMAGE_STATE_H
#define IMAGE_STATE_H

#include <memory>
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
    std::unique_ptr<cv::Mat> Image;
};

#endif // IMAGE_STATE_H
