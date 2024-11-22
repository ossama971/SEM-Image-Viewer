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

void ImageWidgetCore::drawIntensityPlot(int y, int xStart, int xEnd) {
    if (currentImage.empty()) {
        // qDebug() << "Image is not grayscale or is empty.";
        return;
    }
    Mat image = currentImage;
    if (currentImage.type() != CV_8UC1)
    {
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }

    // Extract the intensity values along the specified row (horizontal line)
    std::vector<int> intensities;
    for (int x = xStart; x <= xEnd; ++x) {
        intensities.push_back(image.at<uchar>(y, x));
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
    axisX->setRange(xStart, intensities.size() - 1);
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
        if (intensityPlotMode) {
            graphicsView->setDragMode(QGraphicsView::NoDrag); // Disable drag in intensity plot mode
            static bool customCursorSet = false;
            if (!customCursorSet) {
                QPixmap cursorPixmap(":/icons/pen-icon.svg");
                QSize cursorSize(20, 20); // Adjust size as needed
                QPixmap scaledCursor = cursorPixmap.scaled(cursorSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                graphicsView->setCursor(QCursor(scaledCursor));
                customCursorSet = true;
            }


            if (event->type() == QEvent::GraphicsSceneMousePress) {
                auto mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
                if (mouseEvent) {
                    // Capture starting point for the line
                    lineStart = mouseEvent->scenePos();
                    qDebug() << "Mouse Press at:" << lineStart;

                    // Delete the previous line from the scene
                    if (intensityLine) {
                        scene->removeItem(intensityLine);
                        delete intensityLine;
                        intensityLine = nullptr;
                    }
                    // Create a temporary line item if it doesn't exist
                    if (!intensityLine) {
                        intensityLine = new QGraphicsLineItem();
                        intensityLine->setPen(QPen(Qt::red, 2));
                        scene->addItem(intensityLine);
                    }

                    // Initialize the line position
                    intensityLine->setLine(lineStart.x(), lineStart.y(), lineStart.x(), lineStart.y());
                    return true;
                }
            } else if (event->type() == QEvent::GraphicsSceneMouseMove) {
                auto mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
                if (mouseEvent && intensityLine) {
                    // Update the line's endpoint in real-time, keeping the Y-coordinate fixed
                    QPointF scenePos = mouseEvent->scenePos();
                    intensityLine->setLine(lineStart.x(), lineStart.y(), scenePos.x(), lineStart.y());

                    // Update mouse position on the info bar
                    infoBar->setMousePosition(static_cast<int>(scenePos.x()),
                                              static_cast<int>(scenePos.y()));
                    return true;
                }
            } else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
                auto mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
                if (mouseEvent && intensityLine) {
                    // Finalize the line
                    lineEnd = mouseEvent->scenePos();
                    qDebug() << "Mouse Release at:" << lineEnd;

                    // Set the final line position

                    intensityLine->setLine(lineStart.x(), lineStart.y(), lineEnd.x(), lineStart.y());

                    // Trigger intensity plot drawing
                    qreal xStart = lineStart.x();
                    qreal xEnd = lineEnd.x();
                    if (xStart > xEnd) {
                        std::swap(xStart, xEnd);
                    }
                    drawIntensityPlot(static_cast<int>(lineStart.y()),
                                      static_cast<int>(xStart),
                                      static_cast<int>(xEnd));

                    // Optional: Clear the temporary line if not needed after plotting
                    // delete intensityLine;
                    intensityLine = nullptr;
                    graphicsView->unsetCursor();
                    customCursorSet = false;


                    return true;
                }
            }
        }



    if (obj == scene && event->type() == QEvent::GraphicsSceneMouseMove) {
      QGraphicsSceneMouseEvent *mouseEvent =
          static_cast<QGraphicsSceneMouseEvent *>(event);
      QPointF scenePos = mouseEvent->scenePos();

      infoBar->setMousePosition(static_cast<int>(scenePos.x()),
                                static_cast<int>(scenePos.y()));

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
