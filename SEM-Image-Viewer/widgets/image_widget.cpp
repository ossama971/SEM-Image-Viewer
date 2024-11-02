#include "image_widget.h"
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QScrollBar>
#include <QGraphicsSceneMouseEvent>

using namespace cv;
using namespace std;

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), graphicsView(new QGraphicsView(this)), scene(new QGraphicsScene(this)), zoomWidget(new ZoomWidget(this))
{
    setMinimumSize(600, 400);
    graphicsView->setScene(scene);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(graphicsView);
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

    // Connect zoomWidget signals to ImageWidget's zoom slots
    connect(zoomWidget, &ZoomWidget::zoomInRequested, this, &ImageWidget::zoomIn);
    connect(zoomWidget, &ZoomWidget::zoomOutRequested, this, &ImageWidget::zoomOut);
}

void ImageWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    loadAndDisplayImage("/Users/osama/Developer/SiemensFinalProj/SEM-Image-Viewer/assets/micro-electronic-sed.jpg");
    scene->installEventFilter(this);
}

bool ImageWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == scene && event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
        if (mouseEvent) {
            QPointF clickPos = mouseEvent->scenePos();
            qDebug() << "Clicked position in scene:" << clickPos;
        }
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void ImageWidget::loadAndDisplayImage(const QString &imagePath)
{
    auto pixmap = loadAndPrepareImage(imagePath, graphicsView->size());
    if (pixmap) {
        setImage(*pixmap);
    } else {
        emit imageLoadFailed();
    }
}

optional<QPixmap> ImageWidget::loadAndPrepareImage(const QString &path, const QSize &targetSize)
{
    Mat image = imread(path.toStdString());
    if (image.empty()) {
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
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    // Reposition the zoom widget at the top-right corner
    if (zoomWidget) {
        zoomWidget->move(width() - zoomWidget->width() - 10, 10);
    }
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
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
    if (isPanning) {
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
    if (event->button() == Qt::LeftButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void ImageWidget::wheelEvent(QWheelEvent *event)
{
    const qreal zoomIncrement = 1.1;
    qreal factor = event->angleDelta().y() > 0 ? zoomIncrement : 1 / zoomIncrement;
    zoomFactor *= factor;

    // Limit the zoom factor to prevent excessive zooming
    zoomFactor = qBound(0.1, zoomFactor, 10.0);
    QPointF viewCenter = graphicsView->mapToScene(graphicsView->viewport()->rect().center());
    graphicsView->scale(factor, factor);
    graphicsView->centerOn(viewCenter);
    event->accept();
}

void ImageWidget::zoomIn()
{
    graphicsView->scale(1.15, 1.15);
}

void ImageWidget::zoomOut()
{
    graphicsView->scale(1 / 1.15, 1 / 1.15);
}
