#ifndef GRAY_SCALE_FILTER_H
#define GRAY_SCALE_FILTER_H

#include "image_filter.h"

class GrayScaleFilter : public ImageFilter {
public:
    bool applyFilter(const Image &inputImage, cv::Mat &outputImage, bool log = true) const override;

    ImageStateSource getImageSource() const override;
};

#endif // GRAY_SCALE_FILTER_H
