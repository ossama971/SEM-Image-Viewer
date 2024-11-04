#ifndef DENOISE_FILTER_H
#define DENOISE_FILTER_H

#include "ImageFilter.h"

class NoiseReductionFilter : public ImageFilter {
public:
    NoiseReductionFilter(double intensity);

    cv::Mat applyFilter(const Image& inputImage) const override;

    double getIntensity() const;

private:
    double _intensity;
};

#endif // DENOISE_FILTER_H
