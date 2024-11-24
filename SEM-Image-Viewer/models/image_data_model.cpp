#include "image_data_model.h"
#include <QImage>
#include <QVariant>
#include <opencv2/imgproc.hpp>

#include "../core/engines/workspace.h"
#include <QDebug>

ImageDataModel::ImageDataModel(QObject *parent) : QAbstractListModel(parent) {
    // Connect the repository's signal to the data model's slot
    QObject::connect(&Workspace::Instance()->getActiveSession().getImageRepo(), &ImageRepository::onDirectoryChanged,
                     this, &ImageDataModel::updateImages);


    // Image* selectedImage = Workspace::Instance()->getActiveSession().getImageRepo().getImage();

    // if(!selectedImage){
    //     qDebug()<<"selected image is nullllllllllllllll------------------";
    // }else{
    //     QObject::connect(selectedImage, &Image::onImageStateUpdated,
    //                      this, &ImageDataModel::updateImagesAfterFilter);
    // }

    QObject::connect(&Workspace::Instance()->getActiveSession().getImageRepo(),
                     &ImageRepository::onImageChanged,
                     this, [this](Image* newImage) {
                         if (newImage) {
                             QObject::connect(newImage, &Image::onImageStateUpdated,
                                              this, &ImageDataModel::updateImagesAfterFilter,Qt::UniqueConnection);
                         } else {
                             qDebug() << "New selected image is null; no signal connection.";
                         }
                     });


}

void ImageDataModel::updateImagesAfterFilter(std::vector<std::unique_ptr<ImageState>>& states) {
    qDebug()<<"alooooooooooooo------------------";
    // beginResetModel();
    // m_thumbnails.clear();
    std::vector<Image*> newImages =Workspace::Instance()->getActiveSession().getImageRepo().getImages();
    qDebug()<<newImages.size();
    qDebug()<<"in updated ";
    updateImages("", newImages, false);


    // loadImages(0, 20);
}


void ImageDataModel::updateImages(const std::string newDir, std::vector<Image*> newImages, bool image_load) {
    beginResetModel();
    images.clear();
    m_thumbnails.clear();
    images = QList<Image*>(newImages.begin(), newImages.end());
    qDebug()<<"in directoryyyyy ";
    qDebug()<<images.size();
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

QImage convertMatToQImage(const cv::Mat &mat) {
    if (mat.empty()) {
        return QImage(); // Return an empty QImage if the input cv::Mat is empty
    }

    QImage qImg;

    if (mat.type() == CV_8UC1) {
        // Grayscale image
        qImg = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    } else if (mat.type() == CV_8UC3) {
        // RGB (BGR in OpenCV)
        QImage tempImg(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        qImg = tempImg.rgbSwapped(); // Convert BGR to RGB
    } else {
        qDebug() << "Unsupported image format. Ensure the cv::Mat is CV_8UC1 or CV_8UC3.";
        return QImage(); // Return an empty QImage if the format is unsupported
    }

    return qImg;
}


QImage ImageDataModel::generateThumbnail(const Image &image) const {
    cv::Mat mat = image.getImageMat(); // Updated to use `getImageMat`
    if (mat.empty()) return QImage();

    // Convert cv::Mat (BGR) to QImage (RGB)
    QImage  originalImage = convertMatToQImage(mat);

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
