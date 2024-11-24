#ifndef IMAGE_WIDGET_CORE_H
#define IMAGE_WIDGET_CORE_H

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
#include <QtCharts/QChartView>
#include "../core/data/image.h"
#include "image_info_bar.h"
#include "zoom_widget.h"

class ImageWidgetCore : public QWidget {
  Q_OBJECT

public:
  explicit ImageWidgetCore(QWidget *parent = nullptr);
  void loadAndDisplayImage(const Image &image);
  cv::Mat getImage() const;
  void setIntensityPlotMode(bool enabled);
  void handleHeatmap(const cv::Mat heatmap,bool checked);
  void resetView();
protected:
  void showEvent(QShowEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;


  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  QGraphicsView *graphicsView;
  QGraphicsScene *scene;
  ZoomWidget *zoomWidget;

  QChartView *chartView;
  std::optional<QPixmap> loadAndPrepareImage(const Image &image,
                                             const QSize &targetSize);
  void setImage(const QPixmap &pixmap);
  QPixmap matToQPixmap(cv::Mat image);
  void clearSceneToDefault();
  ImageInfoBar *infoBar;
  cv::Mat currentImage;
  QPoint lastMousePosition;
  bool isPanning = false;
  double zoomFactor = 1.0;
  QGraphicsPixmapItem *image= nullptr;
  QGraphicsPixmapItem *heatmap = nullptr;

  bool intensityPlotMode = false;
  QGraphicsLineItem* intensityLine = nullptr;
  QPointF lineStart;
  QPointF lineEnd;
  int xStart;
  int xEnd;
  void drawIntensityPlot(int y,int xStart, int xEnd);

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



#endif // IMAGE_WIDGET_CORE_H
