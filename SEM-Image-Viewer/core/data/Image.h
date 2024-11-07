#ifndef IMAGE_H
#define IMAGE_H

#include "ImageState.h"
#include "ImageMetadata.h"
#include <list>
#include <QObject>
#include <opencv2/opencv.hpp>

class Image : public QObject {
public:
    Image();
    Image(const Image& image);
    ~Image();

    bool load(const std::string &path);
    bool setImage(cv::Mat image, ImageStateSource newState = ImageStateSource::Origin);

    Image clone();

    cv::Mat getImageMat() const;
    ImageStateSource getImageState() const;
    std::string getPath() const;
    ImageMetadata getMetadata() const;
<<<<<<< Updated upstream
    void setMat(cv::Mat _image);
=======

signals:
    void onImageStateUpdated(std::list<ImageState>& states);

>>>>>>> Stashed changes
private:
    bool _loaded;
    std::string _path;
    ImageMetadata _metadata;
    std::list<ImageState> _states;
};

#endif // IMAGE_H
