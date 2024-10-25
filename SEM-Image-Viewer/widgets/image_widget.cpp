#include "image_widget.h"
#include <QVBoxLayout>
#include <QImageReader>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <optional>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent),
    imageLabel(new QLabel(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);

    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setFixedSize(600, 400);
}

std::optional<QPixmap> ImageWidget::loadAndPrepareImage(const QString &path, const QSize &targetSize)
{
    // Load the image using OpenCV
    cv::Mat image = cv::imread(path.toStdString());
    if (image.empty()) {
        return std::nullopt;
    }

    // Convert to RGB
    cv::Mat rgb;
    cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB);

    // Convert to QImage and then to QPixmap
    QImage qimg(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimg);

    return pixmap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void ImageWidget::loadAndDisplayImage(const QString &imagePath)
{
    std::optional<QPixmap> pixmap = loadAndPrepareImage(imagePath, imageLabel->size());
    if (pixmap) {
        setImage(*pixmap);
    } else {
        emit imageLoadFailed();
    }
}

void ImageWidget::setImage(const QPixmap &pixmap)
{
    imageLabel->setPixmap(pixmap);
}
