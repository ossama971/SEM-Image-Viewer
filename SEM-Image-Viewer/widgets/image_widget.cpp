#include "image_widget.h"
#include <QVBoxLayout>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent),
    imageLabel(new QLabel(this))  // Initialize the label
{
    // Set up the layout to only include the label
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);

    // Center the image in the label
    imageLabel->setFixedSize(600, 400);
    imageLabel->setAlignment(Qt::AlignCenter);
}

void ImageWidget::setImage(const QPixmap &pixmap)
{
    // Set the pixmap to the label
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
