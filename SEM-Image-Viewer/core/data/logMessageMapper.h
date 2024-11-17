#ifndef LOGMESSAGEMAPPER_H
#define LOGMESSAGEMAPPER_H
#include <QString>
#include <QSize>
#include <QImage>

#define LOG_INFO 1

#define LOG_WARNING 2

#define LOG_ERROR 3

class LogMessageMapper {
public:
    // Image Operations
    static QString imageLoaded(const QString& path, const QSize& size);
    static QString imageLoadFailed(const QString& path, const QString& error);
    static QString imageSaved(const QString& path, const QString& format);

    // Filter Operations
    static QString filterApplied(const QString& filterName);
    static QString filterStarted();
    static QString filterCompleted(qint64 processingTime);

    // Image Properties
    static QString imageProperties(const QImage& image);

    // User Interface Events
    static QString userAction(const QString& action, const QString& detail);
    static QString zoomLevel(double zoomFactor);

    // Error Messages
    static QString generalError(const QString& operation);



    // File Operations
    static QString fileOperation(const QString& operation, const QString& path);
};
#endif
