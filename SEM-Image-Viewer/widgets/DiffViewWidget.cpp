#include "DiffViewWidget.h"
#include "../core/image_utils.h"

#include <QDebug>

DiffViewWidget::DiffViewWidget(QWidget *parent)
    : QWidget(parent), upperImageWidget(new ImageWidget(this)),
      lowerImageWidget(new ImageWidget(this)),
      diffImageWidget(new ImageWidget(this)) {

  QVBoxLayout *vlayout = new QVBoxLayout();
  vlayout->addWidget(upperImageWidget);
  vlayout->addWidget(lowerImageWidget);

  QHBoxLayout *hlayout = new QHBoxLayout();
  hlayout->addLayout(vlayout);
  hlayout->addWidget(diffImageWidget);

  setLayout(hlayout);

  upperImageWidget->loadAndDisplayImage(
      "/home/bigfish/wsp/siemens/qt-diff-view/images/window.png");

  lowerImageWidget->loadAndDisplayImage(
      "/home/bigfish/wsp/siemens/qt-diff-view/images/window-with-hand.png");

  connect(upperImageWidget, &ImageWidget::imageUpdated, this,
          &DiffViewWidget::updateDiffImage);

  connect(lowerImageWidget, &ImageWidget::imageUpdated, this,
          &DiffViewWidget::updateDiffImage);

  updateDiffImage();
}

void DiffViewWidget::updateDiffImage() {
  qDebug() << "Updating diff image";

  cv::Mat upperImage = upperImageWidget->getImage();
  cv::Mat lowerImage = lowerImageWidget->getImage();

  if (!upperImage.empty() && !lowerImage.empty() &&
      upperImage.size() == lowerImage.size()) {
    cv::Mat diffImage =
        ImageUtils::diffTwoImages(upperImage, lowerImage, threshold);
    diffImageWidget->updateImage(diffImage);
  }
}
