#ifndef IMAGE_H
#define IMAGE_H

#include "ImageState.h"
#include "ImageMetadata.h"
#include <memory>
#include <vector>
#include <QObject>
#include <opencv2/opencv.hpp>

class Image : public QObject {
    Q_OBJECT

public:
    Image();
    Image(const std::string &path);
    Image(const Image& image);
    ~Image();

    Image operator=(const Image& image);

    bool load(const std::string &path);
    bool setImage(cv::Mat image, ImageStateSource newState = ImageStateSource::Origin);

    //Image clone();

    cv::Mat& getImageMat() const;
    ImageStateSource getImageState() const;
    std::string getPath() const;
    ImageMetadata getMetadata() const;

signals:
    void onImageStateUpdated(std::vector<std::unique_ptr<ImageState>>& states);

private:
    bool _loaded;
    std::string _path;
    ImageMetadata _metadata;
    std::vector<std::unique_ptr<ImageState>> _states;
    std::vector<std::unique_ptr<ImageState>> _undo;
};

#endif // IMAGE_H
