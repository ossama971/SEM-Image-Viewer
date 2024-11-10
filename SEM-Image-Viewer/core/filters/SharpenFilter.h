#ifndef SHARPEN_FILTER_H
#define SHARPEN_FILTER_H

#include "ImageFilter.h"
#include "../engines/Logger.h"
#include "../data/InfoMessage.h"

class SharpenFilter : public ImageFilter {
public:
    cv::Mat applyFilter(const Image& inputImage) const override;

    ImageStateSource getImageSource() const override;
};

#endif // SHARPEN_FILTER_H
