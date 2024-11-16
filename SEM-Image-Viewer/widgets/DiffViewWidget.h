#ifndef DIFFVIEWWIDGET_H
#define DIFFVIEWWIDGET_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include "ImageWidgetCore.h"

class DiffViewWidget : public QWidget {
  Q_OBJECT

public:
  explicit DiffViewWidget(QWidget *parent = nullptr);

private slots:
    void updateDiffImage();

public slots:
  void setImages(const Image &upperImage, const Image &lowerImage);

private:
  ImageWidgetCore *upperImageWidget;
  ImageWidgetCore *lowerImageWidget;
  ImageWidgetCore *diffImageWidget;

  int threshold = 30;

};

#endif // DIFFVIEWWIDGET_H
