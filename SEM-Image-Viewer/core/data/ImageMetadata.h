#ifndef IMAGE_METADATA_H
#define IMAGE_METADATA_H

#include "ImageFormat.h"
#include "ImageColorSpace.h"
#include <string>
#include <time.h>
#include <opencv2/opencv.hpp>

class ImageMetadata {
public:
    void load(const std::string &path, const cv::Mat &image);

private:
    ImageFormat getImageFormat(const std::string &path);
    bool isGreyScale(const cv::Mat &image);

public:
    int getWidth() const;
    int getHeight() const;
    ImageFormat getFormat() const;
    ColorSpace getColorSpace() const;

private:
    int _width, _height;
    ImageFormat _format;
    ColorSpace _colorSpace;
    //time_t _dateCreated;
    time_t _dateModified;
};

#endif // IMAGE_METADATA_H
