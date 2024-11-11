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

  connect(upperImageWidget, &ImageWidget::imageUpdated, this,
          &DiffViewWidget::updateDiffImage);

  connect(lowerImageWidget, &ImageWidget::imageUpdated, this,
          &DiffViewWidget::updateDiffImage);

  updateDiffImage();
}

void DiffViewWidget::updateDiffImage() {
  cv::Mat upperImage = upperImageWidget->getImage();
  cv::Mat lowerImage = lowerImageWidget->getImage();

  if (!upperImage.empty() && !lowerImage.empty() && upperImage.size() == lowerImage.size()) {
    cv::Mat diffImage = ImageUtils::diffTwoImages(upperImage, lowerImage, threshold);
    diffImageWidget->updateImage(diffImage);
  }
}

void DiffViewWidget::setImages(const Image &upperImage, const Image &lowerImage) {
  qDebug() << "Setting images in diff view";
  qDebug() << "Upper image path: " << upperImage.getPath().string().c_str();
  qDebug() << "Lower image path: " << lowerImage.getPath().string().c_str();
  upperImageWidget->loadAndDisplayImage(upperImage);
  lowerImageWidget->loadAndDisplayImage(lowerImage);
  // updateDiffImage();
}
