#include "image_cache_task.h"
#include <filesystem>

ImageCacheTask::ImageCacheTask(const std::string path) : QObject(), _path(path)
{}

void ImageCacheTask::run() {
    if (!std::filesystem::exists(_path))
        return;

    QImage image;
    if (!image.load(QString::fromStdString(_path)))
        return;

    emit imageLoaded(_path, std::move(image));
}
