#ifndef SHARPEN_FILTER_H
#define SHARPEN_FILTER_H

#include "image_filter.h"


class SharpenFilter : public ImageFilter {
public:
    bool applyFilter(const Image &inputImage, cv::Mat &outputImage, bool log = true) const override;

    ImageStateSource getImageSource() const override;
};

#endif // SHARPEN_FILTER_H
