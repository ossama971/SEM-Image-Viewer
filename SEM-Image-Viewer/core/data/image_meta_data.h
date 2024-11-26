#ifndef IMAGE_METADATA_H
#define IMAGE_METADATA_H

#include "image_format.h"
#include "image_color_space.h"
#include "Visitor.h"
#include "Visitable.h"

#include <string>
#include <filesystem>
#include <opencv2/opencv.hpp>

class ImageMetadata : public Visitable {
public:
    void load(const std::string &path, const cv::Mat &image);

private:
    const ImageFormat getImageFormat(const std::string &path);
    bool isGreyScale(const cv::Mat &image);

public:
    const int getWidth() const;
    const int getHeight() const;
    const ImageFormat getFormat() const;
    const ColorSpace getColorSpace(const cv::Mat &image);
    const std::filesystem::file_time_type getDateModified() const;

    void accept(Visitor &v) const override;

private:
    bool _loaded = false;
    int _width, _height;
    ImageFormat _format;
    std::filesystem::file_time_type _dateModified;
};

#endif // IMAGE_METADATA_H
