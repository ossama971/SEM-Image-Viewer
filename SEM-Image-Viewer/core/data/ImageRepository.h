#ifndef IMAGE_REPO_H
#define IMAGE_REPO_H

#include "Image.h"
#include "ImageFormat.h"
#include "Visitor.h"
#include "Visitable.h"

#include <QObject>
#include <string>
#include <vector>

#define IMAGE_FILE_REGEX "^.*[.](png|jpg|bmp)$"

class ImageRepository : public QObject, public Visitable {
    Q_OBJECT

public:
    ImageRepository();

    bool load_directory(const std::string &path);
    bool load_image(const std::string &path);
private:
    void load_image_core(std::unique_ptr<Image> image, const std::string &path, std::vector<std::unique_ptr<Image>>* container);
    int count_images(const std::string &dir);

public:
    bool save(Image& image, const ImageFormat format, const std::string path);

    void selectImage(int index);
    void selectImage(const std::string& path);

    Image* getImage();
    std::vector<Image*> getImages() const;
    std::vector<Image*> getImages(std::vector<int> indices) const;
    std::string getFolderPath() const;

    void accept(Visitor &v) const override;

signals:
    void onImageLoadStarted(int image_count);
    void onImageLoaded(Image* newImage);
    void onDirectoryChanged(const std::string newDir, std::vector<Image*> newImages, bool image_load);
    void onImageChanged(Image* newImage);
    void onImageSaved(const Image& image, const ImageFormat format, const std::string path);

private:
    std::string _folderPath;
    std::vector<std::unique_ptr<Image>> _images;
    Image* _selectedImage;
};

#endif // IMAGE_REPO_H
