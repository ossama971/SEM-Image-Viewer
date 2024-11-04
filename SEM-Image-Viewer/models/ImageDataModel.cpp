#include "ImageDataModel.h"
#include <QImage>
#include <QVariant>
#include <opencv2/imgproc.hpp>

ImageDataModel::ImageDataModel(QObject *parent) : QAbstractListModel(parent) {}

bool ImageDataModel::loadImagesFromPath(const QString &path) {
    QDir directory(path);
    QStringList imageFiles = directory.entryList(QStringList() << "*.png" << "*.jpg" << "*.bmp", QDir::Files);

    if (imageFiles.isEmpty())
        return false;

    beginResetModel();
    images.clear();

    for (const QString &fileName : imageFiles) {
        QString filePath = directory.absoluteFilePath(fileName);
        Image image;
        image.load(filePath.toStdString()); // Use `load` to set the path and read the image
        images.append(image);
    }

    endResetModel();
    return true;
}

int ImageDataModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return images.size();
}

QVariant ImageDataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= images.size())
        return QVariant();

    if (role == Qt::DecorationRole) {
        QImage thumbnail = generateThumbnail(images[index.row()]);
        return thumbnail;
    }
    return QVariant();
}

QImage ImageDataModel::generateThumbnail(const Image &image) const {
    cv::Mat mat = image.getImageMat(); // Updated to use `getImageMat`
    if (mat.empty()) return QImage();

    // Convert cv::Mat (BGR) to QImage (RGB)
    QImage originalImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    originalImage = originalImage.rgbSwapped(); // Convert BGR to RGB

    // Using high-quality scaling
    return originalImage.scaled(thumbnailSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void ImageDataModel::appendImage(const Image& image) {
    beginInsertRows(QModelIndex(), images.size(), images.size());
    images.push_back(image);
    endInsertRows();
}

Image ImageDataModel::getImageAt(int index) const {
    if (index >= 0 && index < images.size()) {
        return images[index];
    }
    return Image(); // Return an empty `Image` if index is out of bounds
}
