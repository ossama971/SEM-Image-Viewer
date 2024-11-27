#ifndef IMAGE_H
#define IMAGE_H

#include "image_state.h"
#include "image_meta_data.h"
#include "Visitor.h"
#include "Visitable.h"
#include "image_cache_pool.h"
#include "../../image_cache_config.h"

#include <memory>
#include <vector>
#include <filesystem>
#include <QObject>
#include <opencv2/opencv.hpp>

class Image : public QObject, public Visitable {
    Q_OBJECT

public:
    Image(ImageCachePool* cachePool);
    Image(ImageCachePool* cachePool, const std::filesystem::path path);
    Image(const Image& image);
    Image(Image&& image);
    Image& operator=(const Image& image);
    Image& operator=(Image&& image);
    virtual ~Image();

    virtual bool load(const std::filesystem::path &path);
    virtual bool save(const std::string &path, ImageState *state);

    virtual bool setImage(cv::Mat *image, const ImageStateSource newState = ImageStateSource::Origin);
    virtual void addRedo(const cv::Mat &imagePath, const ImageStateSource newState);

private:
    bool save(const std::string &path, ImageState *state, const cv::Mat &image);

    ImageCachePool::ImageCacheQuery getImageMat(bool autoLoad) const;
    std::filesystem::path getPath(const ImageStateSource newState) const;

public:
    bool isLoaded() const;
    virtual const cv::Mat& getImageMat() const;
    virtual cv::Mat readImageMat() const;
    ImageStateSource getImageState() const;
    std::filesystem::path getPath() const;
    ImageMetadata getMetadata() const;

    std::vector<std::unique_ptr<ImageState>>const & getStates() const;
    std::vector<std::unique_ptr<ImageState>>const & getUndo() const;

    void accept(Visitor &v) const override;

    const QString getCurrentAction() const;
    bool undo();
    bool redo();
    const QList<QString> getHistory();
    bool isChanged() const;

public slots:
    void onCacheImageLoaded(const std::string &path, cv::Mat *image);

signals:
    void onImageStateUpdated(Image* image);

#ifdef GRID_VIEW_MANUAL_UPDATE
    void onCacheLoaded();
#endif

protected:
    bool _loaded;
    std::filesystem::path _path;
    ImageMetadata _metadata;
    std::vector<std::unique_ptr<ImageState>> _states;
private:
    std::vector<std::unique_ptr<ImageState>> _redo;
    ImageCachePool *_cachePool;
};

cv::Mat loadFromQrc(QString qrc, int flag = cv::IMREAD_COLOR);

#endif // IMAGE_H
