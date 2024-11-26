#ifndef CACHE_POOL_H
#define CACHE_POOL_H

#include <QObject>
#include <mutex>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <opencv2/opencv.hpp>
#include <chrono>

class ImageCachePool : public QObject {
    Q_OBJECT

public:
    ImageCachePool(int maxSize);

private:
    cv::Mat loadFromQrc(QString qrc, int flag = cv::IMREAD_COLOR);

public:
    void setMaxSize(int maxSize);
    void clear();

    bool set(const std::string &path, cv::Mat* image = nullptr);
    bool remove(const std::string &path);

    cv::Mat* get(const std::string &path, bool autoLoad = true);
    cv::Mat* operator[](const std::string &path);

    cv::Mat* getImageLoadingTemplate();

private:
    bool remove(const std::string &path, bool removeAccessHit);
    bool freeSize(int reqSize);

    bool lockFileLoading(const std::string &fileName);
    void unlockFileLoading(const std::string &fileName);

private slots:
    void onImageTaskFinished(const std::string &path, cv::Mat image);

signals:
    void onImageLoaded(const std::string &path, cv::Mat *image);
    void onImageRemoved(const std::string &path, cv::Mat *image);

private:
    using high_resolution_clock = std::chrono::high_resolution_clock;

    struct ImageCacheItem
    {
        std::unique_ptr<cv::Mat> Image;
        int CacheSize;
        high_resolution_clock::time_point InsertTime;
    };

    int _maxSize, _curSize;
    std::mutex _cacheMtx, _loadMtx;
    std::map<std::string, ImageCacheItem> _cache;
    std::map<high_resolution_clock::time_point, std::string> _accessHits;
    std::set<std::string> _filesLoading;

    cv::Mat _imageLoadingTemplate;
};

#endif // CACHE_POOL_H
