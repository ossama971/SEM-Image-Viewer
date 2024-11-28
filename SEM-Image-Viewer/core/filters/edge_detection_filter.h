#ifndef CONTOURS_FILTER_H
#define CONTOURS_FILTER_H

#include "image_filter.h"


class EdgeDetectionFilter : public ImageFilter {
private:
    int threshold_low=50;
    int threshold_high=150;

    cv::Mat denoise(const cv::Mat &image) const;
    cv::Mat toGrayscale(const cv::Mat &image) const;
public:
    EdgeDetectionFilter();
    void setThresholdLow(int threshold);
    void setTHresholdHigh(int threshold);
    int get_threshold_low();
    int get_threshold_high();
    bool applyFilter(const Image &inputImage, cv::Mat &outputImage, bool log = true) const override;

    ImageStateSource getImageSource() const override;
};

#endif // CONTOURS_FILTER_H
