#include "diff_view_widget.h"
#include "../core/utils.h"
#include "../core/engines/workspace.h"
#include "../core/engines/logger.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

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
  QObject::connect(&Workspace::Instance()->getActiveSession().getImageRepo(), &ImageRepository::onDirectoryChanged,
                   this, &DiffViewWidget::resetDiff);
}

void DiffViewWidget::updateDiffImage() {
  cv::Mat upperImage = upperImageWidget->getImage();
  cv::Mat lowerImage = lowerImageWidget->getImage();


  if (upperImage.empty() || lowerImage.empty()) {
      return;
  }
  if ((upperImage.cols != lowerImage.cols) || (upperImage.rows != lowerImage.rows)) {
      diffImageWidget->updateImage(cv::Mat());
      Logger::instance()->logMessage(
          Logger::MessageTypes::warning, Logger::MessageID::no_difference,
          Logger::MessageOption::without_path,
          {});
      return;
  }
      cv::Mat diffImage = Utils::diffTwoImages(upperImage, lowerImage, threshold);
      diffImageWidget->updateImage(diffImage);
}

void DiffViewWidget::setImages(Image *upperImage, Image *lowerImage) {
  upperImageWidget->loadAndDisplayImage(*upperImage);
  lowerImageWidget->loadAndDisplayImage(*lowerImage);
}

void DiffViewWidget::resetDiff(){
  upperImageWidget->resetView();
  lowerImageWidget->resetView();
  diffImageWidget->resetView();
}
