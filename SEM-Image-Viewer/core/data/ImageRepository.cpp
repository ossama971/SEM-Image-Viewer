#include "ImageRepository.h"
#include <filesystem>
#include <string>
#include <regex>
#include <boost/algorithm/string.hpp>

ImageRepository::ImageRepository() : _selectedImage(nullptr)
{
}

bool ImageRepository::load_directory(const std::string &path)
{
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

            std::string filename = it->path().filename().string();
            boost::to_lower(filename);
            if (!std::regex_search(filename, what, filter))
                continue;

            image_path = it->path().string();

            Image img;
            img.load(image_path);

            images.push_back(std::move(img));
        }

        _images = std::move(images);
        emit onDirectoryChanged(path, _images, false);

        return true;
    }
    catch (std::filesystem::filesystem_error ex)
    {
    }
    catch (std::regex_error ex)
    {
    }

    return false;
}

bool ImageRepository::load_image(const std::string &path)
{
    std::filesystem::path fpath(path);
    if (!std::filesystem::exists(fpath))
        return false;

    const std::regex filter(IMAGE_FILE_REGEX);
    std::smatch what;

    if (!std::regex_search(path, what, filter))
        return false;

    Image img;
    if (!img.load(path))
        return false;

    _images.clear();
    _images.push_back(std::move(img));

    emit onDirectoryChanged(fpath.remove_filename().string(), _images, true);
    return true;
}

bool ImageRepository::save(Image &image, const ImageFormat format, const std::string path)
{
    return false;
}

void ImageRepository::selectImage(int index)
{
    if (index == -1)
        _selectedImage = nullptr;
    else if (index < _images.size())
        _selectedImage = &_images[index];

    emit onImageChanged(_selectedImage);
}

void ImageRepository::selectImage(const std::string& path)
{
    int i = 0, image_index = -1;

    for (auto it = _images.begin(); it != _images.end(); ++it, ++i)
    {
        if (!it->getPath().compare(path))
        {
            image_index = i;
            break;
        }
    }

    if (image_index == -1)
        return;

    selectImage(image_index);
}

Image *ImageRepository::getImage()
{
    return _selectedImage;
}

std::vector<Image> ImageRepository::getImages()
{
    return _images;
}
