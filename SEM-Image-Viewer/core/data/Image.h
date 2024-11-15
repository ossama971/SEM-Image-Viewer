#ifndef IMAGE_H
#define IMAGE_H

#include "ImageState.h"
#include "ImageMetadata.h"
#include <memory>
#include <vector>
#include <filesystem>
#include <QObject>
#include <opencv2/opencv.hpp>

class Image : public QObject {
    Q_OBJECT

public:
    Image();
    Image(const std::filesystem::path path);
    Image(const Image& image);
    ~Image();

    Image operator=(const Image& image);

    bool load(const std::filesystem::path path);
    bool setImage(cv::Mat image, ImageStateSource newState = ImageStateSource::Origin);

    //Image clone();

    cv::Mat& getImageMat() const;
    ImageStateSource getImageState() const;
    std::filesystem::path getPath() const;
    ImageMetadata getMetadata() const;

    QString GetCurrentAction() const;
    bool undo();
    bool redo();
    QList<QString> getHistory();

signals:
    void onImageStateUpdated(std::vector<std::unique_ptr<ImageState>>& states);

private:
    bool _loaded;
    std::filesystem::path _path;
    ImageMetadata _metadata;
    std::vector<std::unique_ptr<ImageState>> _states;
    std::vector<std::unique_ptr<ImageState>> _redo;
};

#endif // IMAGE_H
