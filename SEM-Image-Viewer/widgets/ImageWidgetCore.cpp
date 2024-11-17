#include "ImageWidgetCore.h"
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <vector>

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

void ImageWidgetCore::drawIntensityPlot(int y) {
    if (currentImage.empty() || currentImage.type() != CV_8UC1) {
        qDebug() << "Image is not grayscale or is empty.";
        return;
    }

    // Extract the intensity values along the specified row (horizontal line)
    std::vector<int> intensities;
    for (int x = 0; x < currentImage.cols; ++x) {
        intensities.push_back(currentImage.at<uchar>(y, x));
    }

    // Create a QtCharts series for plotting the intensity values
    QLineSeries *series = new QLineSeries();
    for (int x = 0; x < intensities.size(); ++x) {
        series->append(x, intensities[x]);
    }

    // Clear any previous plot items in the scene
    QList<QGraphicsItem *> items = scene->items();
    for (auto *item : items) {
        if (dynamic_cast<QChartView *>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

    // Create a chart and configure it
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Intensity Plot");
    chart->legend()->hide();

    // Customize axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, intensities.size() - 1);
    axisX->setTitleText("X-axis (Pixel)");

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 255);
    axisY->setTitleText("Intensity (Value)");

    // Add axes to the chart
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Attach the series to the axes
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Create a ChartView and embed it in the scene
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Set position for the plot overlay
    QRectF viewRect = graphicsView->sceneRect();
    chartView->setGeometry(viewRect.width() / 4, viewRect.height() / 2, viewRect.width() / 2, viewRect.height() / 3);

    scene->addWidget(chartView);
}

bool ImageWidgetCore::eventFilter(QObject *obj, QEvent *event) {
    if (obj == scene) {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            QGraphicsSceneMouseEvent *mouseEvent =
                dynamic_cast<QGraphicsSceneMouseEvent *>(event);
            if (mouseEvent) {
                QPointF clickPos = mouseEvent->scenePos();
                int y = static_cast<int>(clickPos.y());
                if (intensityPlotMode) {
                    // Draw horizontal line
                    QGraphicsLineItem *line = scene->addLine(0, y, currentImage.cols, y, QPen(Qt::red, 2));
                    line->setZValue(1); // Bring line to the top
                    // Plot the intensity
                    drawIntensityPlot(y);
                }
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
    if (intensityPlotMode && event->button() == Qt::LeftButton) {
        QPointF scenePos = graphicsView->mapToScene(event->pos());
        lineStart = scenePos;
        if (!intensityLine) {
            intensityLine = new QGraphicsLineItem();
            scene->addItem(intensityLine);
        }
        intensityLine->setLine(scenePos.x(), scenePos.y(), scenePos.x(), scenePos.y());
        event->accept();
        return;
    }
      if (event->button() == Qt::LeftButton) {
        isPanning = true;
        lastMousePosition = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
      }
      event->ignore();
    QWidget::mousePressEvent(event);
}

void ImageWidgetCore::mouseMoveEvent(QMouseEvent *event) {
    event->accept();
    if (intensityPlotMode && intensityLine) {
        QPointF scenePos = graphicsView->mapToScene(event->pos());
        intensityLine->setLine(lineStart.x(), lineStart.y(), scenePos.x(), lineStart.y());
        event->accept();
        return;
    }
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
    QWidget::mouseMoveEvent(event);
}

void ImageWidgetCore::mouseReleaseEvent(QMouseEvent *event) {
    if (intensityPlotMode && event->button() == Qt::LeftButton) {
        QPointF scenePos = graphicsView->mapToScene(event->pos());
        lineEnd = scenePos;

        // Ensure the line is horizontal
        intensityLine->setLine(lineStart.x(), lineStart.y(), lineEnd.x(), lineStart.y());

        // Extract intensity values
        if (!currentImage.empty() && currentImage.channels() == 1) {
            int y = static_cast<int>(lineStart.y());
            int xStart = static_cast<int>(std::min(lineStart.x(), lineEnd.x()));
            int xEnd = static_cast<int>(std::max(lineStart.x(), lineEnd.x()));

            std::vector<uchar> intensities;
            for (int x = xStart; x <= xEnd; ++x) {
                if (x >= 0 && x < currentImage.cols && y >= 0 && y < currentImage.rows) {
                    intensities.push_back(currentImage.at<uchar>(y, x));
                }
            }

            qDebug() << "Intensity values along the line:" << intensities;

            // Signal or method call to plot the intensity values
            // emit intensityPlotGenerated(intensities);
        }
        event->accept();
        return;
    }
    if (event->button() == Qt::LeftButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
    QWidget::mouseReleaseEvent(event);
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


void ImageWidgetCore::setIntensityPlotMode(bool enabled) {
    intensityPlotMode = enabled;

    // Clear existing line if the mode is disabled
    if (!enabled && intensityLine) {
        scene->removeItem(intensityLine);
        delete intensityLine;
        intensityLine = nullptr;
    }
}
