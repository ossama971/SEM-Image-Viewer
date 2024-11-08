#include "ImageMetadata.h"
#include <boost/algorithm/string.hpp>

void ImageMetadata::load(const std::string &path, const cv::Mat &image) {
    _width = image.cols;
    _height = image.rows;
    _format = getImageFormat(path);
    _colorSpace = isGreyScale(image) ? ColorSpace::Gray : ColorSpace::RGB;
    _dateModified = std::filesystem::last_write_time(path);

}

ImageFormat ImageMetadata::getImageFormat(const std::string &path) {
    std::string::size_type i = path.find_last_of(".");
    if (i == std::string::npos || i + 1 >= path.length())
        return ImageFormat::Unknown;

    std::string extension(path.substr(i + 1));
    if (extension.length() < 3)
        return ImageFormat::Unknown;

    if (boost::iequals(extension, "PNG"))
        return ImageFormat::PNG;
    else if (boost::iequals(extension, "JPG"))
        return ImageFormat::JPG;
    else if (boost::iequals(extension, "BMP"))
        return ImageFormat::BMP;

    return ImageFormat::Unknown;
}

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

ColorSpace ImageMetadata::getColorSpace(void) const {
    return _colorSpace;
}
