#include "image_widget_core.h"
#include "../core/engines/workspace.h"
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QGraphicsProxyWidget>
#include <vector>
#include <QGraphicsSceneMouseEvent>
#include <QVBoxLayout>


using namespace cv;
using namespace std;

ImageWidgetCore::ImageWidgetCore(QWidget *parent, bool cacheRead)
    : QWidget(parent), graphicsView(new QGraphicsView(this)),
      scene(new QGraphicsScene(this)), zoomWidget(new ZoomWidget(this)),
    infoBar(new ImageInfoBar(this)), readFromCache(cacheRead) {
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
        return;
    }

    Mat image = currentImage;
    if (currentImage.type() != CV_8UC1) {
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }

    // Extract intensity values along the specified row
    std::vector<int> intensities;
    for (int x = xStart; x <= xEnd; ++x) {
        intensities.push_back(image.at<uchar>(y, x));
    }

    // Determine the min and max intensity values
    auto [minIntensity, maxIntensity] = std::minmax_element(intensities.begin(), intensities.end());

    // Create a QtCharts series for plotting the intensity values
    QLineSeries *series = new QLineSeries();
    series->setPen(QPen(Qt::red));
    for (int x = 0; x < intensities.size(); ++x) {
        series->append(xStart + x, intensities[x]); // Match the X-axis to the actual image positions
    }

    // Clear previous plot items in the scene
    QList<QGraphicsItem *> items = scene->items();
    for (auto *item : items) {
        if (dynamic_cast<QChartView *>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }

    // Create and configure the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setBackgroundVisible(false);
    chart->setBackgroundBrush(Qt::NoBrush);
    chart->setPlotAreaBackgroundVisible(false);

    // Customize axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(xStart, xEnd); // Exact X range
    axisX->setLabelsVisible(false);
    axisX->setGridLineVisible(false);
    axisX->setVisible(false);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(*minIntensity, 255); // Dynamic Y range
    axisY->setLabelsVisible(false);
    axisY->setGridLineVisible(false);
    axisY->setVisible(false);

    // Add axes to the chart
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Attach the series to the axes
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Create a ChartView and embed it in the scene
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setAttribute(Qt::WA_TranslucentBackground);
    chartView->setStyleSheet("background: transparent;");

    // Create a proxy widget to embed the chart view into the scene
    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(chartView);

    // Get the pixmap item's position in the scene
    QGraphicsPixmapItem *pixmapItem = dynamic_cast<QGraphicsPixmapItem *>(scene->items().last());
    if (!pixmapItem) {
        return; // No pixmap item found
    }

    // Align the chart in the scene
    QPointF pixmapPos = pixmapItem->sceneBoundingRect().topLeft();
    QRectF pixmapRect = pixmapItem->sceneBoundingRect();

    // Calculate the chart position and size
    double pixelToSceneRatio = pixmapRect.width() / currentImage.cols; // Scale factor
    double chartWidth = (xEnd - xStart + 1) * pixelToSceneRatio; // Match the pixel range
    double chartHeight = pixmapRect.height() / 4.0;             // Proportional height

    double chartX = pixmapPos.x() + (xStart * pixelToSceneRatio); // Align with xStart
    double chartY = pixmapPos.y() + (y * pixelToSceneRatio) - chartHeight; // Position above the row

    QRectF chartRect(chartX, chartY, chartWidth, chartHeight);
    proxyWidget->setGeometry(chartRect);

    // Add the proxy widget to the scene
    if (!scene->items().contains(proxyWidget)) {
        scene->addItem(proxyWidget);
    }
    chartView->setCursor(Qt::OpenHandCursor);
}

bool ImageWidgetCore::eventFilter(QObject *obj, QEvent *event) {
    if (obj == scene) {

        intensityPlotMode = (Workspace::Instance()->getActiveSession().horizontalIntensityPlotMode);

        if (intensityPlotMode && !currentImage.empty()) {
            if (event->type() == QEvent::KeyPress) {
                Workspace::Instance()->getActiveSession().toggleHorizontalPlotMode();
                customClearScene();
            }

            graphicsView->setDragMode(QGraphicsView::NoDrag); // Disable drag in intensity plot mode
            static bool customCursorSet = false;
            if (!customCursorSet) {
                QColor backgroundColor = graphicsView->palette().color(graphicsView->backgroundRole());
                QColor lightModeBackground(255, 255, 255, 255);
                qDebug()<<backgroundColor;
                if (backgroundColor == lightModeBackground)
                {
                    QPixmap cursorPixmap(":/icons/pen-icon.svg");
                    QSize cursorSize(20, 20); // Adjust size as needed
                    QPixmap scaledCursor = cursorPixmap.scaled(cursorSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    graphicsView->setCursor(QCursor(scaledCursor));

                }
                else
                {
                    QPixmap cursorPixmap(":/icons/pen-icon-dark.svg");
                    QSize cursorSize(20, 20); // Adjust size as needed
                    QPixmap scaledCursor = cursorPixmap.scaled(cursorSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    graphicsView->setCursor(QCursor(scaledCursor));

                }
                customCursorSet = true;
            }


            if (event->type() == QEvent::GraphicsSceneMousePress) {
                auto mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
                if (mouseEvent) {
                    isPlotting = true;
                    // Capture starting point for the line
                    lineStart = mouseEvent->scenePos();

                    // Delete the previous line from the scene
                    if(intensityLine)
                        customClearScene();                   
                    intensityLine = new QGraphicsLineItem();
                    intensityLine->setPen(QPen(Qt::blue, 2));
                    scene->addItem(intensityLine);

                    // // Initialize the line position
                    intensityLine->setLine(lineStart.x(), lineStart.y(), lineStart.x(), lineStart.y());
                    return true;
                }
            } else if (event->type() == QEvent::GraphicsSceneMouseMove) {
                auto mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);

                if (mouseEvent && intensityLine && isPlotting) {
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

                    // Adjust xStart and xEnd based on the scene's transformations
                    QGraphicsPixmapItem *pixmapItem = dynamic_cast<QGraphicsPixmapItem *>(scene->items().last());
                    if (pixmapItem) {
                        QPointF pixmapPos = pixmapItem->sceneBoundingRect().topLeft();

                        // Compute xStart and xEnd relative to the pixmap position
                        qreal xStart = lineStart.x() - pixmapPos.x();
                        qreal xEnd = lineEnd.x() - pixmapPos.x();

                        // Ensure xStart is less than xEnd
                        if (xStart > xEnd) {
                            std::swap(xStart, xEnd);
                        }

                        // Finalize the intensity line in the scene
                        intensityLine->setLine(lineStart.x(), lineStart.y(), lineEnd.x(), lineStart.y());

                        // Trigger the intensity plot drawing relative to the pixmap position
                        drawIntensityPlot(static_cast<int>(lineStart.y() - pixmapPos.y()),
                                          static_cast<int>(xStart),
                                          static_cast<int>(xEnd));
                    }

                    // Reset cursor and exit intensity plot mode
                    graphicsView->unsetCursor();
                    customCursorSet = false;
                    isPlotting = false;
                    Workspace::Instance()->getActiveSession().toggleHorizontalPlotMode();
                    return true;
                }
            }
        }
        else
        {
            graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            if (event->type() == QEvent::GraphicsSceneMousePress)
                customClearScene();

        }        

        if (obj == scene && event->type() == QEvent::GraphicsSceneMouseMove) {
            QGraphicsSceneMouseEvent *mouseEvent =
                static_cast<QGraphicsSceneMouseEvent *>(event);

            if (mouseEvent) {
                // Get the scene position of the mouse
                QPointF scenePos = mouseEvent->scenePos();

                // Try to find the last pixmap item in the scene
                QGraphicsPixmapItem *pixmapItem = nullptr;
                for (QGraphicsItem *item : scene->items()) {
                    pixmapItem = dynamic_cast<QGraphicsPixmapItem *>(item);
                    if (pixmapItem) break; // Found a pixmap item, break the loop
                }

                if (pixmapItem) {
                    // Get the top-left corner of the pixmap in scene coordinates
                    QRectF pixmapRect = pixmapItem->sceneBoundingRect();
                    QPointF pixmapPos = pixmapRect.topLeft();

                    // Calculate the relative position of the mouse to the pixmap
                    double relativeX = scenePos.x() - pixmapPos.x();
                    double relativeY = scenePos.y() - pixmapPos.y();

                    // Invert Y-axis: 0 at bottom, height at top
                    double invertedY = pixmapRect.height() - relativeY;

                    // Update the info bar with the relative position
                    infoBar->setMousePosition(static_cast<int>(relativeX),
                                              static_cast<int>(invertedY));
                } else {
                    // No pixmap found: Use the scene position as fallback
                    infoBar->setMousePosition(static_cast<int>(scenePos.x()),
                                              static_cast<int>(scenePos.y()));
                }
            }
        }
  }
  return QWidget::eventFilter(obj, event);
}

void ImageWidgetCore::loadAndDisplayImage(const Image &image) {
  auto pixmap = loadAndPrepareImage((Image*)&image, graphicsView->size());
  if (pixmap) {
    setImage(*pixmap);
    setDimensions(pixmap->width(), pixmap->height());
  } else {
    emit imageLoadFailed();
  }
  emit imageUpdated(currentImage);

}

void ImageWidgetCore::setDimensions(int width, int height) {
    infoBar->setDimensions(width, height);
}

optional<QPixmap>
ImageWidgetCore::loadAndPrepareImage(Image *selected_image,
                                     const QSize &targetSize) {
    const Mat& image = readFromCache ? selected_image->getImageMat() : selected_image->readImageMat();
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
  QPixmap pixmap = QPixmap::fromImage(qImage);
  return pixmap;
}

void ImageWidgetCore::setImage(const QPixmap &pixmap) {
  scene->clear();
  image = scene->addPixmap(pixmap);
  image->setFlags(QGraphicsItem::ItemIsSelectable |
                  QGraphicsItem::ItemIsFocusable |
                  QGraphicsItem::ItemIsMovable);
  image->setAcceptHoverEvents(true);
  image->setAcceptedMouseButtons(Qt::LeftButton);

  const int margin = 50; // Set the desired margin value
  QRectF imageBounds = image->boundingRect();
  QRectF sceneBounds = imageBounds.adjusted(-margin, -margin, margin, margin);
  scene->setSceneRect(sceneBounds);
  graphicsView->fitInView(image, Qt::KeepAspectRatio);

  //graphicsView->setSceneRect(image->boundingRect());
  QSizeF viewSize = graphicsView->viewport()->size();
  qreal scaleX = viewSize.width() / sceneBounds.width();
  qreal scaleY = viewSize.height() / sceneBounds.height();
  zoomFactor = qMin(scaleX, scaleY);
  infoBar->setZoomPercentage(zoomFactor);
  infoBar->setImageSize(pixmap.width(), pixmap.height());
  //Intialize heatmap
  heatmap = nullptr;
}

void ImageWidgetCore::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  if (zoomWidget) {
    zoomWidget->move(width() - zoomWidget->width() - 10, 10);
  }
}

void ImageWidgetCore::wheelEvent(QWheelEvent *event) {
   customClearScene();
  if (event->modifiers() & Qt::ControlModifier) {

    const qreal zoomIncrement = 1.1;
    qreal factor =
        event->angleDelta().y() > 0 ? zoomIncrement : 1 / zoomIncrement;
    zoomFactor *= factor;

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
    customClearScene();
    const qreal zoomIncrement = 1.1;
    qreal factor = zoomIncrement; // Similar to wheelEvent
    zoomFactor *= factor;

    // Optional: Limit the zoom factor to a maximum
    zoomFactor = qMin(zoomFactor, 10.0);

    // Maintain the view center
    QPointF viewCenter = graphicsView->mapToScene(graphicsView->viewport()->rect().center());
    graphicsView->scale(factor, factor);
    graphicsView->centerOn(viewCenter);

    // Update the zoom percentage
    infoBar->setZoomPercentage(zoomFactor);
}

void ImageWidgetCore::zoomOut() {
    customClearScene();
    const qreal zoomIncrement = 1.1;
    qreal factor = 1 / zoomIncrement; // Similar to wheelEvent
    zoomFactor *= factor;

    // Optional: Limit the zoom factor to a minimum
    zoomFactor = qMax(zoomFactor, 0.1);

    // Maintain the view center
    QPointF viewCenter = graphicsView->mapToScene(graphicsView->viewport()->rect().center());
    graphicsView->scale(factor, factor);
    graphicsView->centerOn(viewCenter);

    // Update the zoom percentage
    infoBar->setZoomPercentage(zoomFactor);
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
  setDimensions(image.cols, image.rows);
}

void ImageWidgetCore::onupdateImageState(Image* image) {
  updateImage(readFromCache ? image->getImageMat() : image->readImageMat());
}

void ImageWidgetCore::handleHeatmap(const cv::Mat &image,bool checked)
{
    QPixmap pixmap=matToQPixmap(image);
    if(!heatmap)
    {
        heatmap = new QGraphicsPixmapItem(pixmap, this->image);
        return;
    }

    // Toggle visibility of the heatmap
    if (checked) {
        heatmap->setVisible(true);
    } else {
        heatmap->setVisible(false);
    }

}

const cv::Mat& ImageWidgetCore::getImage() const {
  return currentImage;
}



QPixmap ImageWidgetCore::matToQPixmap(const cv::Mat &image)
{
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
    return pixmap;
}

void ImageWidgetCore::clearSceneToDefault() {
    // Clear all items from the scene
    for (QGraphicsItem *item : scene->items()) {
        delete item;
    }
    scene->clear();
}

void ImageWidgetCore::resetView() {
    clearSceneToDefault(); // Clear the scene and set the default placeholder
    infoBar-> setDimensions(0, 0);
    infoBar->setZoomPercentage(1.0);
    infoBar->setMousePosition(0, 0);
}

void ImageWidgetCore::customClearScene(){
    for (QGraphicsItem *item : scene->items()) {
        if(item->type() == QGraphicsPixmapItem::Type)
            continue;
        delete item;
    }
}
