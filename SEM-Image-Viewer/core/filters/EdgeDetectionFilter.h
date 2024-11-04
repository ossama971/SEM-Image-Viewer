#ifndef CONTOURS_FILTER_H
#define CONTOURS_FILTER_H

#include "ImageFilter.h"

class EdgeDetectionFilter : public ImageFilter {
public:
    EdgeDetectionFilter(double threshold);

    cv::Mat applyFilter(const Image& inputImage) const override;

    double getThreshold() const;

private:
    double _threshold;
};

#endif // CONTOURS_FILTER_H
