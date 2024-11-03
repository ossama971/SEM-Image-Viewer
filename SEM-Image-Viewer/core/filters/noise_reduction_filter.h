#ifndef DENOISE_FILTER_H
#define DENOISE_FILTER_H

#include "image_filter.h"

class NoiseReductionFilter : public Filter {
public:
    NoiseReductionFilter(double intensity);

    cv::Mat applyFilter(const cv::Mat& inputImage) const override;

    double getIntensity() const;

private:
    double _intensity;
};

#endif // DENOISE_FILTER_H
