#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <optional>
#include <QString>
#include <opencv2/opencv.hpp>

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override; // Override showEvent to load the image after widget is shown

private:
    QGraphicsView *graphicsView; // Graphics view for displaying the image
    QGraphicsScene *scene;       // Scene for managing graphics items

    void loadAndDisplayImage(const QString &imagePath);
    std::optional<QPixmap> loadAndPrepareImage(const QString &path, const QSize &targetSize);
    void setImage(const QPixmap &pixmap);

signals:
    void imageLoadFailed(); // Signal emitted when image loading fails
};

#endif // IMAGEWIDGET_H
