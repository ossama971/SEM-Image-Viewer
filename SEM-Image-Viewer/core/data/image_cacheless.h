#ifndef IMAGE_CACHELESS_H
#define IMAGE_CACHELESS_H

#include "image.h"

class ImageCacheless : public Image {
    Q_OBJECT

public:
    ImageCacheless();
    ImageCacheless(const std::filesystem::path path);
    ImageCacheless(const ImageCacheless& image);
    ImageCacheless(ImageCacheless&& image);
    ~ImageCacheless();

public:
    bool load(const std::filesystem::path &path) override;

    bool setImage(cv::Mat* image, const ImageStateSource newState = ImageStateSource::Origin) override;
    void addRedo(const cv::Mat &image, const ImageStateSource newState) override;

    const cv::Mat& getImageMat() const override;
    cv::Mat readImageMat() const override;
};

#endif // IMAGE_CACHELESS_H
