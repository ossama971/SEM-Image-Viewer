#ifndef IMAGE_WIDGET_CORE_H
#define IMAGE_WIDGET_CORE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QShowEvent>
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
  explicit ImageWidgetCore(QWidget *parent = nullptr, bool cacheRead = true);
  void loadAndDisplayImage(const Image &image);
  void setDimensions(int width, int height);
  const cv::Mat& getImage() const;
  void handleHeatmap(const cv::Mat &heatmap,bool checked);
  void resetView();
protected:
  void showEvent(QShowEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  bool readFromCache = true;
  QGraphicsView *graphicsView = nullptr;
  QGraphicsScene *scene = nullptr;
  ZoomWidget *zoomWidget = nullptr;
  QChartView *chartView = nullptr;
  ImageInfoBar *infoBar = nullptr;
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

  std::optional<QPixmap> loadAndPrepareImage(Image *selected_image, const QSize &targetSize);
  void setImage(const QPixmap &pixmap);
  QPixmap matToQPixmap(const cv::Mat &image);
  void clearSceneToDefault();
  void drawIntensityPlot(int y,int xStart, int xEnd);
  QGraphicsProxyWidget *proxyWidget = nullptr;
  bool isPlotting = false;

  void customClearScene();

private slots:
  void zoomIn();
  void zoomOut();

signals:
  void imageLoadFailed();
  void imageUpdated(const cv::Mat &image);

public slots:
  void updateImage(const cv::Mat &image);
  void onupdateImageState(Image* image);
};



#endif // IMAGE_WIDGET_CORE_H
