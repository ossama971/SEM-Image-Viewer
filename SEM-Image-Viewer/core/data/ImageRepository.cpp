#include "ImageRepository.h"
#include <filesystem>
#include <string>
#include <regex>
#include <boost/algorithm/string.hpp>
#include "../engines/Logger.h"
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

        int images_count = count_images(path);
        if (!images_count)
            return false;

        selectImage(-1);
        _images.clear();

        //int _logId=Logger::instance()->logMessageWithProgressBar("I100",{ "Path"},images_count);
        _folderPath = path;
        for (std::filesystem::recursive_directory_iterator it(path); it != std::filesystem::recursive_directory_iterator(); ++it)
        {
            if (!std::filesystem::is_regular_file(it->status()))
                continue;

            std::string filename = it->path().filename().string();
            boost::to_lower(filename);
            if (!std::regex_search(filename, what, filter))
                continue;

            image_path = it->path().string();

            std::unique_ptr<Image> img = std::make_unique<Image>();
            load_image_core(std::move(img), image_path, &_images);
           //Logger::instance()->updateProgressBar(_logId,1);
        }

        emit onDirectoryChanged(path, getImages(), false);

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

    selectImage(-1);
    _images.clear();


    std::unique_ptr<Image> img = std::make_unique<Image>();
    load_image_core(std::move(img), path, &_images);

    emit onDirectoryChanged(fpath.remove_filename().string(), getImages(), true);

    return true;
}

void ImageRepository::load_image_core(std::unique_ptr<Image> image, const std::string &path, std::vector<std::unique_ptr<Image>>* container) {
    image->load(path);

    container->push_back(std::move(image));

}

int ImageRepository::count_images(const std::string &dir) {
    int count = 0;

    try
    {
        const std::regex filter(IMAGE_FILE_REGEX);
        std::smatch what;

        for (std::filesystem::recursive_directory_iterator it(dir); it != std::filesystem::recursive_directory_iterator(); ++it)
        {
            if (!std::filesystem::is_regular_file(it->status()))
                continue;

            std::string filename = it->path().filename().string();
            boost::to_lower(filename);
            if (!std::regex_search(filename, what, filter))
                continue;

            count ++;
        }
    }
    catch (std::filesystem::filesystem_error ex)
    {
    }
    catch (std::regex_error ex)
    {
    }

    return count;
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
        _selectedImage = _images[index].get();

    if (_selectedImage) {

        emit loadActionList(_selectedImage->getHistory());
    }

    emit onImageChanged(_selectedImage);

}

void ImageRepository::selectImage(const std::string& path)
{
    int i = 0, image_index = -1;

    for (auto it = _images.begin(); it != _images.end(); ++it, ++i)
    {
        if (!(*it)->getPath().compare(path))
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

Image *ImageRepository::getImage(const std::filesystem::path &path)
{
    for (auto it = _images.begin(); it != _images.end(); ++it)
    {
      if (!(*it)->getPath().compare(path))
        return it->get();
    }

    return nullptr;
}

std::vector<Image*> ImageRepository::getImages() const {
    std::vector<Image*> images(_images.size());

    for (int i = 0; i < _images.size(); ++i)
        images[i] = _images[i].get();

    return images;
}

std::vector<Image*> ImageRepository::getImages(std::vector<int> indices) const {
    std::vector<Image*> result;
    if (indices.empty())
        return result;

    std::vector<Image*> images = getImages();
    int i = 0;
    int currentImage = 0;

    while (i < images.size() && currentImage < indices.size())
    {
        if (indices[currentImage] == i)
        {
            result.push_back(images[i]);

            ++currentImage;
            ++i;
        }

        else if (indices[currentImage] > i)
            ++i;

        else
            ++currentImage;
    }

    return result;
}

std::string ImageRepository::getFolderPath() const {
    return _folderPath;
}

void ImageRepository::accept(Visitor &v) const {
    v.visit(*this);
}
