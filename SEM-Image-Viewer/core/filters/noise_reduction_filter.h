#ifndef DENOISE_FILTER_H
#define DENOISE_FILTER_H

#include "image_filter.h"


class NoiseReductionFilter : public ImageFilter {
public:
    NoiseReductionFilter(double intensity);

    bool applyFilter(const Image &inputImage, cv::Mat &outputImage, bool log = true) const override;

    ImageStateSource getImageSource() const override;
    const double getIntensity() const;

private:
    double _intensity;
};

#endif // DENOISE_FILTER_H
