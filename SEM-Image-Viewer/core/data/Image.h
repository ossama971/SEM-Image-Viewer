#ifndef IMAGE_H
#define IMAGE_H

#include "ImageMetadata.h"
#include <opencv2/opencv.hpp>

class Image {
public:
    ~Image();

    void load(const std::string &path);
    Image clone();

    cv::Mat getImageMat() const;
    std::string getPath() const;
    ImageMetadata getMetadata() const;
    void setMat(cv::Mat _image);
private:
    cv::Mat _image;
    std::string _path;
    ImageMetadata _metadata;
};

#endif // IMAGE_H
