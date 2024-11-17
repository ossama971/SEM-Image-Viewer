#include "logMessageMapper.h"

QString LogMessageMapper::imageLoaded(const QString& path, const QSize& size) {
    return QString("Image loaded - Path: %1, Size: %2x%3")
    .arg(path)
        .arg(size.width())
        .arg(size.height());
}

QString LogMessageMapper::imageLoadFailed(const QString& path, const QString& error) {
    return QString("Failed to load image - Path: %1, Error: %2")
    .arg(path)
        .arg(error);
}

QString LogMessageMapper::imageSaved(const QString& path, const QString& format) {
    return QString("Image saved - Path: %1, Format: %2").arg(path).arg(format);
}

QString LogMessageMapper::filterApplied(const QString& filterName) {
    return QString("Filter successfully applied. Filter Name: %1").arg(filterName);
}


QString LogMessageMapper::filterStarted() {
    return QString("Starting Batch filter");
}

QString LogMessageMapper::filterCompleted(qint64 processingTime) {
    return QString("Filter completed , Processing time: %1ms").arg(processingTime);
}


QString LogMessageMapper::imageProperties(const QImage& image) {
    return QString("Image properties - Size: %1x%2, Format: %3, Depth: %4")
    .arg(image.width())
        .arg(image.height())
        .arg(image.format())
        .arg(image.depth());
}

QString LogMessageMapper::userAction(const QString& action, const QString& detail) {
    return QString("User action - Action: %1, Detail: %2")
    .arg(action)
        .arg(detail);
}

QString LogMessageMapper::zoomLevel(double zoomFactor) {
    return QString("Zoom level changed - Factor: %1")
    .arg(zoomFactor);
}

QString LogMessageMapper::generalError(const QString& operation) {
    return QString("Error during %1")
    .arg(operation);
}


QString LogMessageMapper::fileOperation(const QString& operation, const QString& path) {
    return QString("File operation - Type: %1, Path: %2")
    .arg(operation)
        .arg(path);
}
