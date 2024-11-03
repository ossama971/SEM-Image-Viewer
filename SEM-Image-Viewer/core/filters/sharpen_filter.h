#ifndef SHARPEN_FILTER_H
#define SHARPEN_FILTER_H

#include "image_filter.h"
#include <opencv2/opencv.hpp>

class SharpenFilter : public Filter {
public:

    // Override the applyFilter method
    cv::Mat applyFilter(const cv::Mat& inputImage) const override;

};

#endif // SHARPEN_FILTER_H
