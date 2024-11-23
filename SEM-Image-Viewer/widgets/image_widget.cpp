#include "image_widget.h"
#include "../core/engines/Workspace.h"

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), core(new ImageWidgetCore(this))
{
   QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(core);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    connect(&Workspace::Instance()->getActiveSession().getImageRepo(), &ImageRepository::onImageChanged, this, &ImageWidget::reload);
}

void ImageWidget::reload()
{
    Image* image = Workspace::Instance()->getActiveSession().getImageRepo().getImage();
    if (!image)
        return;
    connect(image, &Image::onImageStateUpdated, core, &ImageWidgetCore::onupdateImageState, Qt::UniqueConnection);
    this->core->loadAndDisplayImage(*image);
}

void ImageWidget::updateImage(const cv::Mat &image)
{
  this->core->updateImage(image);
}

