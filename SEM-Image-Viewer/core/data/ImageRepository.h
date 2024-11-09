#ifndef IMAGE_REPO_H
#define IMAGE_REPO_H

#include "Image.h"
#include "ImageFormat.h"
#include <QObject>
#include <string>
#include <vector>

class ImageRepository : public QObject {
    Q_OBJECT

public:
    ImageRepository();

    bool load_directory(const std::string &path);
    bool load_image(const std::string &path);
    bool save(Image& image, const ImageFormat format, const std::string path);


    void selectImage(int index);

    Image* getImage();
    std::vector<Image> getImages();
    ~ImageRepository();

signals:
    void onDirectoryChanged(std::vector<Image>& newImages);
    void onImageChanged(Image* newImage);
    void onImageSaved(const Image& image, const ImageFormat format, const std::string path);

private:
    std::string _folderPath;
    std::vector<Image> _images;
    Image* _selectedImage;
};

#endif // IMAGE_REPO_H
