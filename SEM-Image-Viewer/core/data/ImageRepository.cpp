#include "ImageRepository.h"
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#define IMAGE_FILE_REGEX "\.(png|jpg|bmp|)$"

ImageRepository::ImageRepository() : _selectedImage(nullptr) {
    load_directory("D:\\Test");
}

bool ImageRepository::load_directory(const std::string &path) {
    try
    {
        const boost::regex filter(IMAGE_FILE_REGEX);
        boost::smatch what;
        std::string image_path;
        std::vector<Image> images;

        for (boost::filesystem::recursive_directory_iterator it(path + "*.png"); it != boost::filesystem::recursive_directory_iterator(); ++it)
        {
            if (!boost::filesystem::is_regular_file(it->status()))
                continue;

            if (!boost::regex_search(it->path().leaf().string(), what, filter))
                continue;

            image_path = it->path().string();

            Image img;
            img.load(image_path);

            images.push_back(std::move(img));
        }

        _images = std::move(images);
        emit onDirectoryChanged(_images);

        return true;
    }
    catch (boost::filesystem::filesystem_error ex) {
    }
    catch (boost::regex_error ex) {
    }

    return false;
}

bool ImageRepository::load_image(const std::string &path) {
    if (!boost::filesystem::exists(path))
        return false;

    const boost::regex filter(IMAGE_FILE_REGEX);
    boost::smatch what;

    if (!boost::regex_search(path, what, filter))
        return false;

    Image img;
    img.load(path);
    _images.push_back(std::move(img));

    emit onDirectoryChanged(_images);
    return true;
}

bool ImageRepository::save(Image& image, const ImageFormat format, const std::string path) {

}

void ImageRepository::selectImage(int index) {
    if (index == -1)
        _selectedImage = nullptr;
    else if (index < _images.size())
        _selectedImage = &_images[index];

    emit onImageChanged(_selectedImage);
}

Image* ImageRepository::getImage() {
    return _selectedImage;
}
