#ifndef IMAGE_REPO_H
#define IMAGE_REPO_H

#include "image.h"
#include "image_format.h"
#include "image_cache_pool.h"
#include "../../image_cache_config.h"
#include "Visitor.h"
#include "Visitable.h"

#include <QObject>
#include <vector>

#define IMAGE_FILE_REGEX "^.*[.](png|jpg|bmp)$"

class ImageRepository : public QObject, public Visitable {
    Q_OBJECT

public:
    ImageRepository();

    bool load_directory(const std::string &path);
    bool load_image(const std::string &path);
    void pre_directory_change(int image_count, const std::string &dir);
    void post_directory_change(const std::string &newDir, bool image_load);

    void selectImage(int index);
    void selectImage(const std::string& path);

    std::size_t getImagesCount() const;
    Image* getImage();
    Image* getImage(const std::filesystem::path &path);
    Image* getImage(const std::size_t index);

    const std::vector<Image*> getImages() const;
    const std::vector<Image*> getImages(const std::vector<int> &indices) const;

    const std::string getFolderPath() const;

    void accept(Visitor &v) const override;
    bool getHasUnsavedChanges();

    // Costly operations, should only be used for 
    // exporting & saving the sessions only
    std::vector<std::unique_ptr<Image>> cloneImages() const;
    std::unique_ptr<Image> cloneSelectedImage() const;

signals:
    void onDirectoryChanged(const std::string &newDir, const std::vector<Image*> &newImages, bool image_load);
    void onImageChanged(Image* newImage);
    void onImageSaved(const Image& image, const ImageFormat format, const std::string &path);
    void loadActionList(const QList<QString> &actions);
    void updateGridView();

public slots:
    void setUnsavedChanges(Image* image);

private slots:
    void onCacheImageLoaded(const std::string &path, QImage *image, cv::Mat* imageMat);
    void onCacheImageRemoved(const std::string &path, QImage *image, cv::Mat* imageMat);

private:
    std::string _folderPath;
    mutable std::recursive_mutex _mutex;
    // TODO: this should be a map of path to image, to have faster lookups
    std::vector<std::unique_ptr<Image>> _images;
    Image* _selectedImage = nullptr;
    bool _hasUnsavedChanges = false; // Tracks whether there are unsaved changes

#ifdef IMAGE_CACHE
    ImageCachePool _cachePool;
#endif
};

#endif // IMAGE_REPO_H
