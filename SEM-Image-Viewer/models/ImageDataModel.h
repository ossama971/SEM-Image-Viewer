#ifndef IMAGEDATAMODEL_H
#define IMAGEDATAMODEL_H

#include "../core/data/image.h"
#include <QAbstractListModel>
#include <QImage>
#include <QDir>

class ImageDataModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit ImageDataModel(QObject *parent = nullptr);

    bool loadImagesFromPath(const QString &path);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    QList<Image> images;
    QSize thumbnailSize = QSize(100, 100);

    QImage generateThumbnail(const Image &image) const;
};

#endif // IMAGEDATAMODEL_H
