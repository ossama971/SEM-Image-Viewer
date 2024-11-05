#ifndef IMAGE_SESSION_H
#define IMAGE_SESSION_H

#include "ImageRepository.h"
#include "UndoManager.h"

class ImageSession {
public:
    void loadImage(const std::string path);
    void saveImage(const std::string path, ImageFormat format);
    cv::Mat applyFilter(std::unique_ptr<ImageFilter> filter);

    cv::Mat undo();
    cv::Mat redo();

    Image& getImage();
    const ImageMetadata getImageInfo();
    std::vector<int> pixelIntensity(const std::vector<std::pair<int, int>> &points);
    cv::Mat heatMap();
    cv::Mat diffTwoImages(const cv::Mat &image2, const int threshold);

private:
    ImageRepository _imageRepo;
    UndoManager _undoManager;
};

#endif // IMAGE_SESSION_H
