#include "image_widget.h"
#include "../core/engines/Workspace.h"

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), core(new ImageWidgetCore(this))
{
   QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(core);
    setLayout(layout); 

    connect(&Workspace::Instance()->getActiveSession().getImageRepo(), &ImageRepository::onImageChanged, this, &ImageWidget::reload);
    // connect(imagerepo->getImage(), &Image::onImageStateUpdated, this, &ImageWidget::reload, Qt::UniqueConnection);
}

void ImageWidget::reload()
{
    Image* image = Workspace::Instance()->getActiveSession().getImageRepo().getImage();
    if (!image)
        return;
    //disconnect(this, SIGNAL(onImageStateUpdated()), nullptr, nullptr);
    connect(image, &Image::onImageStateUpdated, core, &ImageWidgetCore::onupdateImageState, Qt::UniqueConnection);
    this->core->loadAndDisplayImage(*image);
}

void ImageWidget::updateImage(const cv::Mat &image)
{
  this->core->updateImage(image);
}

