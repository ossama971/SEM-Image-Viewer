#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include <opencv2/opencv.hpp>

class Filter {
public:

    virtual cv::Mat applyFilter(const cv::Mat& inputImage) const = 0;

    virtual ~Filter() = default;
};

#endif // IMAGE_FILTER_H
