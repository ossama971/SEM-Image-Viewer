#ifndef IMAGE_SESSION_H
#define IMAGE_SESSION_H

#include "ImageRepository.h"
#include "../filters/ImageFilter.h"
#include <QObject.h>

class SessionData : public QObject {
     Q_OBJECT
public:
    void loadDirectory(const std::string path);
    void loadImage(const std::string path);
    void saveImage(const std::string path, ImageFormat format);
    void applyFilter(std::unique_ptr<ImageFilter> filter);

    bool undo();
    bool redo();

    std::vector<int> pixelIntensity(const std::vector<std::pair<int, int>> &points);
    cv::Mat heatMap();
    cv::Mat diffTwoImages(const cv::Mat &image2, const int threshold);

    ImageRepository& getImageRepo();
    Image* getSelectedImage();

signals:
    void loadActionList(QList<QString> actions);
    void updateActionList(QString action);
    void popActionList();
private:
    ImageRepository _imageRepo;
    //UndoManager _undoManager;
};

#endif // IMAGE_SESSION_H
