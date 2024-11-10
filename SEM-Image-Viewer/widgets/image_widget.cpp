#include "image_widget.h"
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QScrollBar>
#include <QGraphicsSceneMouseEvent>

using namespace cv;
using namespace std;

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), graphicsView(new QGraphicsView(this)), scene(new QGraphicsScene(this)),
      zoomWidget(new ZoomWidget(this)), infoBar(new ImageInfoBar(this))
{
    // setMinimumSize(600, 400);
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
    graphicsView->setMouseTracking(true); // Enable mouse tracking
    graphicsView->installEventFilter(this);

    // Connect zoomWidget signals to ImageWidget's zoom slots
    connect(zoomWidget, &ZoomWidget::zoomInRequested, this, &ImageWidget::zoomIn);
    connect(zoomWidget, &ZoomWidget::zoomOutRequested, this, &ImageWidget::zoomOut);
}

void ImageWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // loadAndDisplayImage("/home/bigfish/wsp/siemens/sem-image-viewer/SEM-Image-Viewer/assets/micro-electronic-sed.jpg");
    scene->installEventFilter(this);
}



bool ImageWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == scene)
    {
        if (event->type() == QEvent::GraphicsSceneMousePress)
        {
            QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
            if (mouseEvent)
            {
                QPointF clickPos = mouseEvent->scenePos();
            }
            return true;
        }
        if (obj == scene && event->type() == QEvent::GraphicsSceneMouseMove)
        {
            QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
            QPointF scenePos = mouseEvent->scenePos();

            // Update info bar with real-time mouse position
            infoBar->setMousePosition(static_cast<int>(scenePos.x()), static_cast<int>(scenePos.y()));

            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ImageWidget::loadAndDisplayImage(const QString &imagePath)
{
    auto pixmap = loadAndPrepareImage(imagePath, graphicsView->size());
    if (pixmap)
    {
        setImage(*pixmap);
        infoBar->setDimensions(pixmap->width(), pixmap->height()); // Set image dimensions
    }
    else
    {
        emit imageLoadFailed();
    }
    emit imageUpdated(currentImage);
}

optional<QPixmap> ImageWidget::loadAndPrepareImage(const QString &path, const QSize &targetSize)
{
    Mat image = imread(path.toStdString());
    currentImage=image;
    if (image.empty())
    {
        return nullopt;
    }

    QImage qImage = QImage(image.data, image.cols, image.rows, image.step[0], QImage::Format_RGB888).rgbSwapped();
    QPixmap pixmap = QPixmap::fromImage(qImage.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    return pixmap;
}

void ImageWidget::setImage(const QPixmap &pixmap)
{
    scene->clear();
    QGraphicsPixmapItem *image = scene->addPixmap(pixmap);
    image->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable);
    image->setAcceptHoverEvents(true);
    image->setAcceptedMouseButtons(Qt::LeftButton);
    graphicsView->fitInView(image, Qt::KeepAspectRatio);
    graphicsView->setSceneRect(image->boundingRect());
    infoBar->setImageSize(pixmap.width(), pixmap.height());
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // loadAndDisplayImage("/Users/osama/Developer/SiemensFinalProj/SEM-Image-Viewer/assets/micro-electronic-sed.jpg");
    graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    // Reposition the zoom widget at the top-right corner
    if (zoomWidget)
    {
        zoomWidget->move(width() - zoomWidget->width() - 10, 10);
    }
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isPanning = true;
        lastMousePosition = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    if (isPanning)
    {
        QPoint delta = lastMousePosition - event->pos();
        graphicsView->horizontalScrollBar()->setValue(graphicsView->horizontalScrollBar()->value() + delta.x());
        graphicsView->verticalScrollBar()->setValue(graphicsView->verticalScrollBar()->value() + delta.y());
        lastMousePosition = event->pos();
        event->accept();
        return;
    }
    event->ignore();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void ImageWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        const qreal zoomIncrement = 1.1;
        qreal factor = event->angleDelta().y() > 0 ? zoomIncrement : 1 / zoomIncrement;
        zoomFactor *= factor;

        // Limit the zoom factor to prevent excessive zooming
        zoomFactor = qBound(0.1, zoomFactor, 10.0);
        QPointF viewCenter = graphicsView->mapToScene(graphicsView->viewport()->rect().center());
        graphicsView->scale(factor, factor);
        graphicsView->centerOn(viewCenter);
        infoBar->setZoomPercentage(zoomFactor); // Update zoom percentage
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void ImageWidget::zoomIn()
{
    const qreal zoomIncrement = 1.1;
    zoomFactor *= zoomIncrement;

    // Limit zoom factor to a maximum
    zoomFactor = qMin(zoomFactor, 10.0);

    graphicsView->setTransform(QTransform().scale(zoomFactor, zoomFactor));
    infoBar->setZoomPercentage(zoomFactor); // Update zoom percentage
}

void ImageWidget::zoomOut()
{
    const qreal zoomIncrement = 1.1;
    zoomFactor /= zoomIncrement;

    // Limit zoom factor to a minimum
    zoomFactor = qMax(zoomFactor, 0.1);

    graphicsView->setTransform(QTransform().scale(zoomFactor, zoomFactor));
    infoBar->setZoomPercentage(zoomFactor); // Update zoom percentage
}

void ImageWidget::updateImage(const cv::Mat &image)
{
    currentImage=image;
    QImage qImage = QImage(image.data, image.cols, image.rows, image.step[0], QImage::Format_RGB888).rgbSwapped();
    QPixmap pixmap = QPixmap::fromImage(qImage);
    setImage(pixmap);
    emit imageUpdated(currentImage);
}

cv::Mat ImageWidget::getImage() const
{
    return currentImage; // Return the stored cv::Mat image
}
