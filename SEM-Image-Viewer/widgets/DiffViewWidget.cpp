#include "DiffViewWidget.h"
#include "../core/image_utils.h"

#include <QDebug>

DiffViewWidget::DiffViewWidget(QWidget *parent)
    : QWidget(parent), upperImageWidget(new ImageWidgetCore(this)),
      lowerImageWidget(new ImageWidgetCore(this)),
      diffImageWidget(new ImageWidgetCore(this)) {

  QVBoxLayout *vlayout = new QVBoxLayout();
  vlayout->addWidget(upperImageWidget);
  vlayout->addWidget(lowerImageWidget);

  QHBoxLayout *hlayout = new QHBoxLayout();
  hlayout->addLayout(vlayout);
  hlayout->addWidget(diffImageWidget);

  setLayout(hlayout);

  connect(upperImageWidget, &ImageWidgetCore::imageUpdated, this,
          &DiffViewWidget::updateDiffImage);

  connect(lowerImageWidget, &ImageWidgetCore::imageUpdated, this,
          &DiffViewWidget::updateDiffImage);

}

void DiffViewWidget::updateDiffImage() {
  qDebug() << "Updating diff image";
  cv::Mat upperImage = upperImageWidget->getImage();
  cv::Mat lowerImage = lowerImageWidget->getImage();

  if (!upperImage.empty() && !lowerImage.empty() && upperImage.size() == lowerImage.size()) {
    cv::Mat diffImage = ImageUtils::diffTwoImages(upperImage, lowerImage, threshold);
    diffImageWidget->updateImage(diffImage);
  }
}

void DiffViewWidget::setImages(Image *upperImage, Image *lowerImage) {
  qDebug() << "Setting images in diff view";
  qDebug() << "Upper image path: " << upperImage->getPath().string().c_str();
  qDebug() << "Lower image path: " << lowerImage->getPath().string().c_str();
  upperImageWidget->loadAndDisplayImage(*upperImage);
  lowerImageWidget->loadAndDisplayImage(*lowerImage);
}
