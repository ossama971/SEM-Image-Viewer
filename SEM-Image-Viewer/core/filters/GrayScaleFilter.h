#ifndef GRAY_SCALE_FILTER_H
#define GRAY_SCALE_FILTER_H

#include "ImageFilter.h"

class GrayScaleFilter : public ImageFilter {
public:
    cv::Mat applyFilter(const Image& inputImage) const override;

    ImageStateSource getImageSource() const override;
};

#endif // GRAY_SCALE_FILTER_H
