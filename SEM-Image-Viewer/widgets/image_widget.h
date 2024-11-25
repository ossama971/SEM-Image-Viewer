#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include "image_widget_core.h"

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);

private:
    ImageWidgetCore *core;

private slots:
    void reload();

public slots:
  void updateImage(const cv::Mat &image);
  void handleHeatmap(const cv::Mat &heatmap,bool checked);

};

#endif // IMAGEWIDGET_H
