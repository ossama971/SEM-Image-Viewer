#include "image_widget.h"
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
using namespace cv;

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), graphicsView(new QGraphicsView(this)), scene(new QGraphicsScene(this))
{
    setMinimumSize(600, 400);
    graphicsView->setScene(scene);  // Set the scene for the graphics view

    // Layout to manage the graphics view within the widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(graphicsView);
    setLayout(layout);
    graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    graphicsView->setAlignment(Qt::AlignCenter);
}

void ImageWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event); // Call base class implementation
    loadAndDisplayImage("/Users/osama/Developer/SiemensFinalProj/SEM-Image-Viewer/assets/micro-electronic-sed.jpg");
}

void ImageWidget::loadAndDisplayImage(const QString &imagePath)
{
    // Attempt to load and prepare the image
    auto pixmap = loadAndPrepareImage(imagePath, graphicsView->size());
    if (pixmap) {
        setImage(*pixmap);
    } else {
        emit imageLoadFailed();  // Emit a signal if loading failed
    }
}

std::optional<QPixmap> ImageWidget::loadAndPrepareImage(const QString &path, const QSize &targetSize)
{
    Mat image = imread(path.toStdString()); // Load image using OpenCV
    if (image.empty()) {
        return std::nullopt;  // Return an empty optional if loading failed
    }

    QImage qImage = QImage(image.data, image.cols, image.rows, image.step[0], QImage::Format_RGB888).rgbSwapped();
    QPixmap pixmap = QPixmap::fromImage(qImage.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    return pixmap;
}

void ImageWidget::setImage(const QPixmap &pixmap)
{
    scene->clear();
    QGraphicsPixmapItem *image = scene->addPixmap(pixmap);
    graphicsView->fitInView(image, Qt::KeepAspectRatio);
    graphicsView->setSceneRect(image->boundingRect());
}
