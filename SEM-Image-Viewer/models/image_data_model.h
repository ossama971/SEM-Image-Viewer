#ifndef IMAGE_DATA_MODEL_H
#define IMAGE_DATA_MODEL_H

#include "../core/data/image.h"
#include <QAbstractListModel>
#include <QImage>
#include <QDir>
#include <QHash>

class ImageDataModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit ImageDataModel(QObject *parent = nullptr);
    Image* getImageAt(int index) const; // Function to get image at a specific index
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void loadImages(int startIndex, int endIndex);  // New function to load images in chunks
    int getImageIndex(Image* image);

public slots:
    void updateImages(const std::string &newDir, const std::vector<Image*> &newImages, bool image_load);  // Slot to receive images from ImageRepository

private slots:
    void onReload();
    void onImageStateUpdated(Image* image);

private:
    class ImageRepository* imageRepo = nullptr;
    QList<Image*> images;
    QSize thumbnailSize = QSize(80, 80);
    mutable QHash<int, QImage> m_thumbnails; // Cache for thumbnails
    QImage generateThumbnail(const Image &image) const;
};

#endif // IMAGE_DATA_MODEL_H
