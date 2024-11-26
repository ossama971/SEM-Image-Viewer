#ifndef CACHE_TASK_H
#define CACHE_TASK_H

#include <QObject>
#include <QRunnable>
#include <string>
#include <QImage>

class ImageCacheTask : public QObject, public QRunnable {
    Q_OBJECT

public:
    ImageCacheTask(const std::string path);

    void run() override;

signals:
    void imageLoaded(const std::string& path, QImage image);

private:
    std::string _path;
};

#endif // CACHE_TASK_H
