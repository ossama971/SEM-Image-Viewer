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

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void loadAndDisplayImage(const QString &imagePath);

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
    ZoomWidget *zoomWidget;  // Separate zoom widget for zoom controls

    std::optional<QPixmap> loadAndPrepareImage(const QString &path, const QSize &targetSize);
    void setImage(const QPixmap &pixmap);


    QPoint lastMousePosition;
    bool isPanning = false;
    double zoomFactor = 1.0;

private slots:
    void zoomIn();
    void zoomOut();

signals:
    void imageLoadFailed();
};

#endif // IMAGEWIDGET_H
