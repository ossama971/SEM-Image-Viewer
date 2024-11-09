#include "ImageRepository.h"
#include <filesystem>
#include <string>
#include <regex>

#define IMAGE_FILE_REGEX "\.(png|jpg|bmp|)$"

ImageRepository::ImageRepository() : _selectedImage(nullptr) {
    load_directory("/Users/salmaabdelghany/Siemens/FinalProject/newload");
}

bool ImageRepository::load_directory(const std::string &path) {
    try
    {
        const std::regex filter(IMAGE_FILE_REGEX);
        std::smatch what;
        std::string image_path;
        std::vector<Image> images;

        for (std::filesystem::recursive_directory_iterator it(path); it != std::filesystem::recursive_directory_iterator(); ++it)
        {
            if (!std::filesystem::is_regular_file(it->status()))
                continue;

            const std::string filename = it->path().filename().string();
            if (!std::regex_search(filename, what, filter))
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
    catch (std::filesystem::filesystem_error ex) {
    }
    catch (std::regex_error ex) {
    }

    return false;
}

bool ImageRepository::load_image(const std::string &path) {
    if (!std::filesystem::exists(path))
        return false;

    const std::regex filter(IMAGE_FILE_REGEX);
    std::smatch what;

    if (!std::regex_search(path, what, filter))
        return false;

    Image img;
    img.load(path);
    _images.push_back(std::move(img));

    emit onDirectoryChanged(_images);
    return true;
}

bool ImageRepository::save(Image& image, const ImageFormat format, const std::string path) {
    return false;
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

std::vector<Image> ImageRepository::getImages(){
    return _images;
}
