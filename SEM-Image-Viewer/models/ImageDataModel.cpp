#include "ImageDataModel.h"
#include <QImage>
#include <QVariant>
#include <opencv2/imgproc.hpp>

#include "../core/engines/Workspace.h"
#include <QDebug>

ImageDataModel::ImageDataModel(QObject *parent) : QAbstractListModel(parent) {
    // Connect the repository's signal to the data model's slot
    QObject::connect(&Workspace::Instance()->getActiveSession().getImageRepo(), &ImageRepository::onDirectoryChanged,
                     this, &ImageDataModel::updateImages);
}

void ImageDataModel::updateImages(const std::string newDir, std::vector<Image*> newImages, bool image_load) {
    beginResetModel();
    images.clear();
    m_thumbnails.clear();
    images = QList<Image*>(newImages.begin(), newImages.end());
    endResetModel();
    loadImages(0, 20);

}

int ImageDataModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return images.size();
}

QVariant ImageDataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= images.size())
        return QVariant();

    if (role == Qt::DecorationRole) {
        // If thumbnail is already cached, return it
        if (m_thumbnails.contains(index.row())) {
            return m_thumbnails[index.row()];
        }

        // Otherwise, generate and cache the thumbnail
        QImage thumbnail = generateThumbnail(*images[index.row()]);
        m_thumbnails.insert(index.row(), thumbnail);
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

void ImageDataModel::loadImages(int startIndex, int endIndex) {
    // Ensure I'm loading valid indices
    endIndex = qMin(endIndex, images.size() - 1);
    if (startIndex > endIndex) return;

    // Load thumbnails for the specified range of images
    for (int i = startIndex; i <= endIndex; ++i) {
        if (!m_thumbnails.contains(i)) {
            QImage thumbnail = generateThumbnail(*images[i]);
            m_thumbnails.insert(i, thumbnail);
        }
    }
}

Image* ImageDataModel::getImageAt(int index) const {
    if (0 <= index && index < images.size()) {
        qDebug() << "ImageDataModel::getImageAt() -> " << index;
        qDebug() << "ImageDataModel::getImageAt() -> " << images[index]->getPath().string().c_str();
        return images[index];
    }
    return nullptr; // Return a default/empty Image if index is out of bounds
}
