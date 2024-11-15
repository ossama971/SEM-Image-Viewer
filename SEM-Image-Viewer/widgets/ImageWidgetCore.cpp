#include "ImageWidgetCore.h"
#include <QDebug>

using namespace cv;
using namespace std;

ImageWidgetCore::ImageWidgetCore(QWidget *parent)
    : QWidget(parent), graphicsView(new QGraphicsView(this)),
      scene(new QGraphicsScene(this)), zoomWidget(new ZoomWidget(this)),
      infoBar(new ImageInfoBar(this)) {
  graphicsView->setScene(scene);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(graphicsView);
  layout->addWidget(infoBar);
  setLayout(layout);

  graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
  graphicsView->setAlignment(Qt::AlignCenter);
  graphicsView->setAttribute(Qt::WA_AcceptTouchEvents);
  graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
  graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  graphicsView->setInteractive(true);
  graphicsView->setMouseTracking(true);
  graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
  graphicsView->setMouseTracking(true);
  graphicsView->installEventFilter(this);

  connect(zoomWidget, &ZoomWidget::zoomInRequested, this,
          &ImageWidgetCore::zoomIn);
  connect(zoomWidget, &ZoomWidget::zoomOutRequested, this,
          &ImageWidgetCore::zoomOut);
}

void ImageWidgetCore::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  scene->installEventFilter(this);
}

bool ImageWidgetCore::eventFilter(QObject *obj, QEvent *event) {
  if (obj == scene) {
    if (event->type() == QEvent::GraphicsSceneMousePress) {
      QGraphicsSceneMouseEvent *mouseEvent =
          dynamic_cast<QGraphicsSceneMouseEvent *>(event);
      if (mouseEvent) {
        QPointF clickPos = mouseEvent->scenePos();
      }
      return true;
    }
    if (obj == scene && event->type() == QEvent::GraphicsSceneMouseMove) {
      QGraphicsSceneMouseEvent *mouseEvent =
          static_cast<QGraphicsSceneMouseEvent *>(event);
      QPointF scenePos = mouseEvent->scenePos();

      infoBar->setMousePosition(static_cast<int>(scenePos.x()),
                                static_cast<int>(scenePos.y()));

#if 0
      Image *image =
          Workspace::Instance()->getActiveSession().getImageRepo().getImage();
      if (image) {
        Mat img = image->getImageMat();
        int x = scenePos.x();
        int y = scenePos.y();
        if (img.type() == CV_8UC3) {
          cv::Vec3b intensity = img.at<cv::Vec3b>(y, x);
          uchar blue = intensity[0];
          uchar green = intensity[1];
          uchar red = intensity[2];

          // Use qDebug to print BGR intensity values with proper formatting
          qDebug() << "BGR intensity at (" << x << "," << y << "): "
                   << "B=" << blue << "G=" << green << "R=" << red;
        } else if (img.type() == CV_8UC1) {
          // Grayscale image (8-bit single channel)
          uchar intensity = img.at<uchar>(y, x);
          qDebug() << "Grayscale intensity (float) at (" << x << ", " << y
                   << "): " << intensity;
        }

        return true;
      }
#endif
    }
  }
  return QWidget::eventFilter(obj, event);
}
void ImageWidgetCore::loadAndDisplayImage(const Image &image) {
  qDebug() << "Loading and displaying image";
  auto pixmap = loadAndPrepareImage(image, graphicsView->size());
  if (pixmap) {
    setImage(*pixmap);
    infoBar->setDimensions(pixmap->width(), pixmap->height());
  } else {
    emit imageLoadFailed();
  }
  emit imageUpdated(currentImage);
}

optional<QPixmap>
ImageWidgetCore::loadAndPrepareImage(const Image &selected_image,
                                     const QSize &targetSize) {
  Mat image = selected_image.getImageMat();
  currentImage = image;
  if (image.empty()) {
    return nullopt;
  }
  QImage qImage;
  if (image.channels() == 1) {
    // Grayscale image: Use QImage::Format_Grayscale8
    qImage = QImage(image.data, image.cols, image.rows, image.step[0],
                    QImage::Format_Grayscale8);
  } else if (image.channels() == 3) {
    // RGB image: Use QImage::Format_RGB888 and swap if necessary
    qImage = QImage(image.data, image.cols, image.rows, image.step[0],
                    QImage::Format_RGB888)
                 .rgbSwapped();
  }
  // QPixmap pixmap = QPixmap::fromImage(qImage.scaled(targetSize,
  // Qt::KeepAspectRatio, Qt::SmoothTransformation));
  QPixmap pixmap = QPixmap::fromImage(qImage);
  return pixmap;
}

void ImageWidgetCore::setImage(const QPixmap &pixmap) {
  scene->clear();
  QGraphicsPixmapItem *image = scene->addPixmap(pixmap);
  image->setFlags(QGraphicsItem::ItemIsSelectable |
                  QGraphicsItem::ItemIsFocusable |
                  QGraphicsItem::ItemIsMovable);
  image->setAcceptHoverEvents(true);
  image->setAcceptedMouseButtons(Qt::LeftButton);
  // graphicsView->fitInView(image, Qt::KeepAspectRatio);
  graphicsView->setSceneRect(image->boundingRect());
  infoBar->setImageSize(pixmap.width(), pixmap.height());
}

void ImageWidgetCore::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  if (zoomWidget) {
    zoomWidget->move(width() - zoomWidget->width() - 10, 10);
  }
}

void ImageWidgetCore::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    isPanning = true;
    lastMousePosition = event->pos();
    setCursor(Qt::ClosedHandCursor);
    event->accept();
    return;
  }
  event->ignore();
}

void ImageWidgetCore::mouseMoveEvent(QMouseEvent *event) {
  event->accept();
  if (isPanning) {
    QPoint delta = lastMousePosition - event->pos();
    graphicsView->horizontalScrollBar()->setValue(
        graphicsView->horizontalScrollBar()->value() + delta.x());
    graphicsView->verticalScrollBar()->setValue(
        graphicsView->verticalScrollBar()->value() + delta.y());
    lastMousePosition = event->pos();
    event->accept();
    return;
  }
  event->ignore();
}

void ImageWidgetCore::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    isPanning = false;
    setCursor(Qt::ArrowCursor);
    event->accept();
    return;
  }
  event->ignore();
}

void ImageWidgetCore::wheelEvent(QWheelEvent *event) {
  if (event->modifiers() & Qt::ControlModifier) {
    const qreal zoomIncrement = 1.1;
    qreal factor =
        event->angleDelta().y() > 0 ? zoomIncrement : 1 / zoomIncrement;
    zoomFactor *= factor;

    // Limit the zoom factor to prevent excessive zooming
    // zoomFactor = qBound(0.1, zoomFactor, 10.0);
    QPointF viewCenter =
        graphicsView->mapToScene(graphicsView->viewport()->rect().center());
    graphicsView->scale(factor, factor);
    graphicsView->centerOn(viewCenter);
    infoBar->setZoomPercentage(zoomFactor); // Update zoom percentage
    event->accept();
  } else {
    event->ignore();
  }
}

void ImageWidgetCore::zoomIn() {
  const qreal zoomIncrement = 1.1;
  zoomFactor *= zoomIncrement;

  // Limit zoom factor to a maximum
  zoomFactor = qMin(zoomFactor, 10.0);

  graphicsView->setTransform(QTransform().scale(zoomFactor, zoomFactor));
  infoBar->setZoomPercentage(zoomFactor); // Update zoom percentage
}

void ImageWidgetCore::zoomOut() {
  const qreal zoomIncrement = 1.1;
  zoomFactor /= zoomIncrement;

  // Limit zoom factor to a minimum
  zoomFactor = qMax(zoomFactor, 0.1);

  graphicsView->setTransform(QTransform().scale(zoomFactor, zoomFactor));
  infoBar->setZoomPercentage(zoomFactor); // Update zoom percentage
}

void ImageWidgetCore::updateImage(const cv::Mat &image) {
  currentImage = image;

  QImage qImage;
  if (image.channels() == 1) {
    // Grayscale image: Use QImage::Format_Grayscale8
    qImage = QImage(image.data, image.cols, image.rows, image.step[0],
                    QImage::Format_Grayscale8);
  } else if (image.channels() == 3) {
    // RGB image: Use QImage::Format_RGB888 and swap if necessary
    qImage = QImage(image.data, image.cols, image.rows, image.step[0],
                    QImage::Format_RGB888)
                 .rgbSwapped();
  }
  QPixmap pixmap = QPixmap::fromImage(qImage);
  setImage(pixmap);
}

void ImageWidgetCore::onupdateImageState(
    std::vector<std::unique_ptr<ImageState>> &states) {
  updateImage(states.back()->Image);
}

cv::Mat ImageWidgetCore::getImage() const {
  return currentImage;
}
