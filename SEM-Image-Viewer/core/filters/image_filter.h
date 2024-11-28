#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include "../data/image.h"

class ImageFilter {
protected:
    ImageFilter() = default;
public:
    virtual ~ImageFilter() = default;

    virtual bool applyFilter(const Image& inputImage, cv::Mat& outputImage, bool log = true) const = 0;

    virtual ImageStateSource getImageSource() const = 0;
};

#endif // IMAGE_FILTER_H
