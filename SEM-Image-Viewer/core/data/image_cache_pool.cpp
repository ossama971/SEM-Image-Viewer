#include "image_cache_pool.h"
#include "image_cache_task.h"
#include "../utils.h"
#include <QFile>
#include <QThreadPool>
#include <filesystem>

#define CACHE_IMAGE_SIZE 1

ImageCachePool::ImageCachePool(int maxSize) : _maxSize(maxSize), _curSize(0) {
    _imageLoadingTemplate = Utils::loadFromQrc(":/assets/image-loading.png", cv::IMREAD_COLOR);
}

void ImageCachePool::setMaxSize(int maxSize) {
    clear();

    _maxSize = maxSize;
}

void ImageCachePool::clear() {
    std::lock_guard<std::mutex> guard(_cacheMtx);

    _curSize = 0;
    _cache.clear();
    _accessHits.clear();
}

bool ImageCachePool::set(const std::string &path, cv::Mat *image, bool notify) {
    std::unique_lock<std::mutex> guard(_cacheMtx);

    const int reqSize = CACHE_IMAGE_SIZE;

    cv::Mat img;
    if (!image)
    {
        img = cv::imread(path);
        if (img.empty())
            return false;

        image = &img;
    }

    high_resolution_clock::time_point time = high_resolution_clock::now();
    auto it = _cache.find(path);
    if (it != _cache.end())
    {
        cv::Mat* imagePtr = it->second.Image.get();
        *imagePtr = *image;

        _accessHits.erase(it->second.InsertTime);
        _accessHits[time] = path;

        guard.unlock();
        if (notify)
            emit onImageLoaded(path, imagePtr);
        return true;
    }

    freeSize(reqSize);
    if (_curSize + reqSize > _maxSize)
        return false;

    std::unique_ptr<cv::Mat> imageUniquePtr = std::make_unique<cv::Mat>(*image);
    cv::Mat *imagePtr = imageUniquePtr.get();

    _cache.insert(std::make_pair(path, ImageCacheItem{ std::move(imageUniquePtr), reqSize, time }));
    _accessHits[time] = path;
    _curSize += reqSize;

    guard.unlock();
    if (notify)
        emit onImageLoaded(path, imagePtr);
    return true;
}

bool ImageCachePool::remove(const std::string &path) {
    std::lock_guard<std::mutex> guard(_cacheMtx);
    return remove(path, true);
}

ImageCachePool::ImageCacheQuery ImageCachePool::get(const std::string &path, bool autoLoad) {
    std::lock_guard<std::mutex> guard(_cacheMtx);

    auto it = _cache.find(path);
    if (it != _cache.end())
        return ImageCacheQuery { it->second.Image.get() };

    if (autoLoad && std::filesystem::exists(path) && lockFileLoading(path))
    {
        ImageCacheTask* task = new ImageCacheTask(path);
        connect(task, &ImageCacheTask::imageLoaded, this, &ImageCachePool::onImageTaskFinished);

        QThreadPool::globalInstance()->start(task);
    }
    return ImageCacheQuery { nullptr };
}

ImageCachePool::ImageCacheQuery ImageCachePool::operator[](const std::string &path) {
    return get(path);
}

ImageCachePool::ImageCacheQuery ImageCachePool::getImageLoadingTemplate(void) {
    return ImageCacheQuery { &_imageLoadingTemplate };
}

bool ImageCachePool::remove(const std::string &path, bool removeAccessHit) {
    auto it = _cache.find(path);
    if (it != _cache.end())
    {
        emit onImageRemoved(it->first, it->second.Image.get());

        if (removeAccessHit)
            _accessHits.erase(it->second.InsertTime);

        _curSize -= it->second.CacheSize;
        _cache.erase(it);
        return true;
    }
    return false;
}

bool ImageCachePool::freeSize(int reqSize) {
    if (_curSize + reqSize > _maxSize)
    {
        int extraSize = _curSize + reqSize - _maxSize;

        while (!_accessHits.empty() && extraSize)
        {
            auto lessRecentlyAccessed = _accessHits.begin();

            if (remove(lessRecentlyAccessed->second, false))
                --extraSize;

            _accessHits.erase(lessRecentlyAccessed);
        }
        return true;
    }
    return false;
}

bool ImageCachePool::lockFileLoading(const std::string &fileName) {
    std::lock_guard<std::mutex> guard(_loadMtx);
    if (_filesLoading.find(fileName) != _filesLoading.end())
        return false;

    _filesLoading.insert(fileName);
    return true;
}

void ImageCachePool::unlockFileLoading(const std::string &fileName) {
    std::lock_guard<std::mutex> guard(_loadMtx);
    _filesLoading.erase(fileName);
}

void ImageCachePool::onImageTaskFinished(const std::string &path, cv::Mat image) {
    set(path, &image, true);
    unlockFileLoading(path);
}
