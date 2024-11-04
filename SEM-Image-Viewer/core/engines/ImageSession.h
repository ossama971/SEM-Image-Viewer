#ifndef IMAGE_SESSION_H
#define IMAGE_SESSION_H

#include "ImageRepository.h"
#include "UndoManager.h"

class ImageSession {
public:
    void loadImage(const std::string path);
    void saveImage(const std::string path, ImageFormat format);
    void applyFilter(std::unique_ptr<ImageFilter> filter);

    void undo();
    void redo();

    const ImageMetadata getImageInfo();
    std::vector<int> pixelIntensity(const std::vector<std::pair<int, int>> &points);
    cv::Mat heatMap();
    cv::Mat diffTwoImages(const cv::Mat &image2, const int threshold);

private:
    ImageRepository _imageRepo;
    UndoManager _undoManager;
};

#endif // IMAGE_SESSION_H
