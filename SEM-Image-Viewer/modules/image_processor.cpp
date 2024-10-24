#include "image_processor.h"
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <iostream>

ImageProcessor::ImageProcessor()
{
    // Constructor implementation (if needed)
}

std::optional<QPixmap> ImageProcessor::loadAndPrepareImage(const std::string &imagePath, const QSize &size)
{
    // Load image using OpenCV
    cv::Mat image = cv::imread(imagePath);

    if (image.empty()) {
        return std::nullopt;  // Return empty optional if the image could not be loaded
    }

    // Convert the image from BGR (OpenCV format) to RGB (Qt format)
    cv::Mat rgbImage;
    cv::cvtColor(image, rgbImage, cv::COLOR_BGR2RGB);

    // Create a QImage from the cv::Mat
    QImage qImage(rgbImage.data, rgbImage.cols, rgbImage.rows, rgbImage.step, QImage::Format_RGB888);

    // Scale the image to fit the specified size while maintaining the aspect ratio
    QPixmap pixmap = QPixmap::fromImage(qImage).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return pixmap;  // Return the processed QPixmap
}
