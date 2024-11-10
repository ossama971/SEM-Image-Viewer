#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPinchGesture>
#include <QWheelEvent>
#include <optional>
#include <QMouseEvent>
#include "zoom_widget.h"
#include <opencv2/opencv.hpp>
#include "../core/data/ImageRepository.h"
#include "image_info_bar.h" // Include the new widget
#include "../core/data/Image.h"

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void loadAndDisplayImage(const Image &image);
    cv::Mat getImage() const;
    ImageRepository *imagerepo = new ImageRepository;
protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;


private:
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    ZoomWidget *zoomWidget; // Separate zoom widget for zoom controls

    std::optional<QPixmap> loadAndPrepareImage(const Image &image, const QSize &targetSize);
    void setImage(const QPixmap &pixmap);


    ImageInfoBar *infoBar; // New info bar widget
    cv::Mat currentImage;
    QPoint lastMousePosition;
    bool isPanning = false;
    double zoomFactor = 1.0;


private slots:
    void zoomIn();
    void zoomOut();
    void reload();

signals:
    void imageLoadFailed();
public slots:
    void updateImage(const cv::Mat &image);
};

#endif // IMAGEWIDGET_H
