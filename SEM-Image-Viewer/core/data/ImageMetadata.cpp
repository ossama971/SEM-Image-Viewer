#include "ImageMetadata.h"

bool ImageMetadata::isGreyScale(const cv::Mat &image) {
    if (image.empty())
        return false;

    cv::Vec3b color;
    cv::Point pt;

    for (int i = 0; i < image.cols; ++i)
    {
        for (int b = 0; b < image.rows; ++b)
        {
            pt.x = i;
            pt.y = b;
            color = image.at<cv::Vec3b>(pt);

            // Gray scale: R == G == B
            if ((color[0] & color[1] & color[2]) != color[0])
                return false;
        }
    }
    return true;
}

ColorSpace ImageMetadata::getColorSpace(void) const
{
    return _colorSpace;
}
