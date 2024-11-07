#ifndef IMAGE_SESSION_H
#define IMAGE_SESSION_H

#include "ImageRepository.h"
#include "UndoManager.h"
#include <QObject.h>

class ImageSession : public QObject {
public:
    void loadDirectory(const std::string path);
    void loadImage(const std::string path);
    void saveImage(const std::string path, ImageFormat format);
    void applyFilter(std::unique_ptr<ImageFilter> filter);

    cv::Mat undo();
    cv::Mat redo();

    std::vector<int> pixelIntensity(const std::vector<std::pair<int, int>> &points);
    cv::Mat heatMap();
    cv::Mat diffTwoImages(const cv::Mat &image2, const int threshold);

    ImageRepository& getImageRepo();
    Image* getSelectedImage();

signals:
    void onImageStateUpdated(const ImageState& newState);

private:
    ImageRepository _imageRepo;
    UndoManager _undoManager;
};

#endif // IMAGE_SESSION_H
