#ifndef IMAGEWIDGETCORE_H
#define IMAGEWIDGETCORE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPinchGesture>
#include <QScrollBar>
#include <QShowEvent>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include <optional>

#include "../core/data/Image.h"
#include "image_info_bar.h"
#include "zoom_widget.h"

class ImageWidgetCore : public QWidget {
  Q_OBJECT

public:
  explicit ImageWidgetCore(QWidget *parent = nullptr);
  void loadAndDisplayImage(const Image &image);
  cv::Mat getImage() const;

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
  ZoomWidget *zoomWidget;

  std::optional<QPixmap> loadAndPrepareImage(const Image &image,
                                             const QSize &targetSize);
  void setImage(const QPixmap &pixmap);

  ImageInfoBar *infoBar;
  cv::Mat currentImage;
  QPoint lastMousePosition;
  bool isPanning = false;
  double zoomFactor = 1.0;

private slots:
  void zoomIn();
  void zoomOut();

signals:
  void imageLoadFailed();
  void imageUpdated(const cv::Mat &image);

public slots:
  void updateImage(const cv::Mat &image);
  void onupdateImageState(std::vector<std::unique_ptr<ImageState>> &states);
};

#endif // IMAGEWIDGETCORE_H
