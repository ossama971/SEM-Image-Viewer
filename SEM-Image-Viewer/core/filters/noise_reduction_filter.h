#ifndef DENOISE_FILTER_H
#define DENOISE_FILTER_H

#include "image_filter.h"
#include "../engines/logger.h"


class NoiseReductionFilter : public ImageFilter {
public:
    NoiseReductionFilter(double intensity);

    cv::Mat applyFilter(const Image& inputImage) const override;

    ImageStateSource getImageSource() const override;
    double getIntensity() const;

private:
    double _intensity;
};

#endif // DENOISE_FILTER_H
