#include "image_info_bar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QSpacerItem>

ImageInfoBar::ImageInfoBar(QWidget *parent)
    : QWidget(parent),
    dimensionsLabel(new QLabel(this)),
    zoomLabel(new QLabel(this)),
    positionLabel(new QLabel(this)),
    imageWidth(0),    // Initialize image width
    imageHeight(0)    // Initialize image height
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);

    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout->addWidget(dimensionsLabel);
    layout->addWidget(zoomLabel);
    layout->addWidget(positionLabel);
    setLayout(layout);

    setMinimumHeight(30);
    setStyleSheet("ImageInfoBar { border: 2px solid black; border-radius: 5px; background-color: #f0f0f0; }");

    setDimensions(0, 0);
    setZoomPercentage(1.0);
    setMousePosition(0, 0);
}

void ImageInfoBar::setDimensions(int width, int height)
{
    dimensionsLabel->setText(QString("Dimensions: %1 x %2").arg(width).arg(height));
}

void ImageInfoBar::setZoomPercentage(double zoom)
{
    zoomLabel->setText(QString("Zoom: %1%").arg(static_cast<int>(zoom * 100)));
}

void ImageInfoBar::setMousePosition(int x, int y)
{
    // Limit the position to the image size
    int limitedX = qBound(0, x, imageWidth - 1);
    int limitedY = qBound(0, y, imageHeight - 1);

    positionLabel->setText(QString("Position: (%1, %2)").arg(limitedX).arg(limitedY));
    positionLabel->repaint(); // Force label to update immediately
}

void ImageInfoBar::setImageSize(int width, int height)
{
    imageWidth = width;
    imageHeight = height;
}
