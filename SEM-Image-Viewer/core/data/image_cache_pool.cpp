#include "image_cache_pool.h"
#include "image_cache_task.h"
#include <QFile>
#include <QThreadPool>
#include <filesystem>

#define CACHE_IMAGE_SIZE   1

ImageCachePool::ImageCachePool(int maxSize) : _maxSize(maxSize), _curSize(0) {
    _imageLoadingTemplate = loadFromQrc(":/assets/image-loading.png", "png");
    _imageLoadingTemplateMat = imageToMat(_imageLoadingTemplate);
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

bool ImageCachePool::set(const std::string &path, cv::Mat* image) {
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
        cv::Mat* imageMatPtr = it->second.ImageMat.get();
        *imageMatPtr = *image;

        QImage* imagePtr = it->second.Image.get();
        *imagePtr = matToImage(*imageMatPtr);

        _accessHits.erase(it->second.InsertTime);
        _accessHits[time] = path;

        guard.unlock();
        emit onImageLoaded(path, imagePtr, imageMatPtr);
        return true;
    }

    freeSize(reqSize);
    if (_curSize + reqSize > _maxSize)
        return false;

    std::unique_ptr<cv::Mat> imageMatUniquePtr = std::make_unique<cv::Mat>(*image);
    std::unique_ptr<QImage> imageUniquePtr = std::make_unique<QImage>(matToImage(*imageMatUniquePtr));
    cv::Mat* imageMatPtr = imageMatUniquePtr.get();
    QImage* imagePtr = imageUniquePtr.get();

    _cache.insert(std::make_pair(path, ImageCacheItem{ std::move(imageUniquePtr), std::move(imageMatUniquePtr), reqSize, time }));
    _accessHits[time] = path;
    _curSize += reqSize;

    guard.unlock();
    emit onImageLoaded(path, imagePtr, imageMatPtr);
    return true;
}

bool ImageCachePool::set(const std::string &path, QImage* image) {
    std::unique_lock<std::mutex> guard(_cacheMtx);

    const int reqSize = CACHE_IMAGE_SIZE;

    QImage img;
    if (!image)
    {
        if (!img.load(QString::fromStdString(path)))
            return false;

        image = &img;
    }

    high_resolution_clock::time_point time = high_resolution_clock::now();
    auto it = _cache.find(path);
    if (it != _cache.end())
    {
        QImage* imagePtr = it->second.Image.get();
        *imagePtr = *image;

        cv::Mat* imageMatPtr = it->second.ImageMat.get();
        *imageMatPtr = imageToMat(*imagePtr);

        _accessHits.erase(it->second.InsertTime);
        _accessHits[time] = path;

        guard.unlock();
        emit onImageLoaded(path, imagePtr, imageMatPtr);
        return true;
    }

    freeSize(reqSize);
    if (_curSize + reqSize > _maxSize)
        return false;

    std::unique_ptr<QImage> imageUniquePtr = std::make_unique<QImage>(*image);
    std::unique_ptr<cv::Mat> imageMatUniquePtr = std::make_unique<cv::Mat>(imageToMat(*imageUniquePtr));
    QImage* imagePtr = imageUniquePtr.get();
    cv::Mat* imageMatPtr = imageMatUniquePtr.get();

    _cache.insert(std::make_pair(path, ImageCacheItem{ std::move(imageUniquePtr), std::move(imageMatUniquePtr), reqSize, time }));
    _accessHits[time] = path;
    _curSize += reqSize;

    guard.unlock();
    emit onImageLoaded(path, imagePtr, imageMatPtr);
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
        return ImageCacheQuery { it->second.Image.get(), it->second.ImageMat.get() };

    if (autoLoad && std::filesystem::exists(path) && lockFileLoading(path))
    {
        ImageCacheTask* task = new ImageCacheTask(path);
        connect(task, &ImageCacheTask::imageLoaded, this, &ImageCachePool::onImageTaskFinished);

        QThreadPool::globalInstance()->start(task);
    }
    return ImageCacheQuery { nullptr, nullptr };
}

ImageCachePool::ImageCacheQuery ImageCachePool::operator[](const std::string &path) {
    return get(path);
}

ImageCachePool::ImageCacheQuery ImageCachePool::getImageLoadingTemplate(void) {
    return ImageCacheQuery { &_imageLoadingTemplate, &_imageLoadingTemplateMat };
}

bool ImageCachePool::remove(const std::string &path, bool removeAccessHit) {
    auto it = _cache.find(path);
    if (it != _cache.end())
    {
        emit onImageRemoved(it->first, it->second.Image.get(), it->second.ImageMat.get());

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

void ImageCachePool::onImageTaskFinished(const std::string &path, QImage image) {
    set(path, &image);
    unlockFileLoading(path);
}

QImage ImageCachePool::loadFromQrc(const QString &qrc, const char *extension) {
    QFile file(qrc);
    QImage img;

    if(file.open(QIODevice::ReadOnly))
        img.load(&file, extension);

    return img;
}

cv::Mat ImageCachePool::imageToMat(const QImage &image) {
    if (image.isNull())
        return cv::Mat();

    switch (image.format())
    {
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::Mat result;

        cv::cvtColor(mat, result, cv::COLOR_BGRA2BGR);
        return result;
    }

    case QImage::Format_RGB888:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    case QImage::Format_Indexed8:
    {
        if (!image.colorTable().isEmpty())
        {
            // Convert to a color-mapped image (indexed colors with a palette)
            QImage colorMappedImage = image.convertToFormat(QImage::Format_RGB888);
            cv::Mat mat(colorMappedImage.height(), colorMappedImage.width(), CV_8UC3, (void*)colorMappedImage.constBits(), colorMappedImage.bytesPerLine());
            return mat.clone();  // Return a deep copy of the color-mapped image
        }

        // If no color table, treat as grayscale and convert to CV_8UC1
        cv::Mat mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    case QImage::Format_RGB16:
    {
        cv::Mat mat(image.height(), image.width(), CV_16UC3, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    case QImage::Format_ARGB8565_Premultiplied:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    case QImage::Format_Grayscale8:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        return mat.clone();
    }

    default:
    {
        // If we encounter any other format, convert to ARGB32 and proceed
        QImage converted = image.convertToFormat(QImage::Format_ARGB32);
        cv::Mat mat(converted.height(), converted.width(), CV_8UC4, (void*)converted.constBits(), converted.bytesPerLine());
        return mat.clone();
    }
    }
}

QImage ImageCachePool::matToImage(const cv::Mat &image) {
    if (image.empty())
        return QImage();

    // Convert from CV_8UC3 (BGR format) to QImage::Format_RGB888
    if (image.channels() == 3)
    {
        QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }

    // Convert from CV_8UC1 (grayscale) to QImage::Format_Indexed8
    else if (image.channels() == 1)
        return QImage(image.data, image.cols, image.rows, image.step, QImage::Format_Indexed8);

    // Convert from CV_8UC4 (BGRA format) to QImage::Format_ARGB32
    else if (image.channels() == 4)
        return QImage(image.data, image.cols, image.rows, image.step, QImage::Format_ARGB32);

    // If the input cv::Mat is an unsupported format
    return QImage();
}
