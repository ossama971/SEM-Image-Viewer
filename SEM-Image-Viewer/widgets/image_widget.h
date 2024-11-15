#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPinchGesture>
#include <QWheelEvent>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>

#include "ImageWidgetCore.h"

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

};

#endif // IMAGEWIDGET_H
