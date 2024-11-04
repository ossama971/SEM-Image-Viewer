#ifndef IMAGE_REPO_H
#define IMAGE_REPO_H

#include "../data/Image.h"
#include "../data/ImageFormat.h"
#include <string>

class ImageRepository {
public:
    void load(const std::string &path);
    bool save(Image image, ImageFormat format);

    Image& getImage();

private:
    Image _image;
};

#endif // IMAGE_REPO_H
