#ifndef DIFF_VIEW_WIDGET_H
#define DIFF_VIEW_WIDGET_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include "image_widget_core.h"

class DiffViewWidget : public QWidget {
  Q_OBJECT

public:
  explicit DiffViewWidget(QWidget *parent = nullptr);

private slots:
    void updateDiffImage();

public slots:
  void setImages(Image *upperImage, Image *lowerImage);
  void resetDiff();

private:
  ImageWidgetCore *upperImageWidget;
  ImageWidgetCore *lowerImageWidget;
  ImageWidgetCore *diffImageWidget;

  int threshold = 30;

};

#endif // DIFF_VIEW_WIDGET_H
