#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include "../data/Image.h"

class ImageFilter {
protected:
    ImageFilter() = default;
public:
    virtual ~ImageFilter() = default;

    virtual cv::Mat applyFilter(const Image& inputImage) const = 0;

    virtual ImageStateSource getImageSource() const = 0;
};

#endif // IMAGE_FILTER_H
