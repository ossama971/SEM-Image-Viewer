#include "ImageRepository.h"
#include "../engines/ThreadPool.h"

#include <filesystem>
#include <string>
#include <regex>

#include <boost/algorithm/string.hpp>

ImageRepository::ImageRepository() : _selectedImage(nullptr)
{

}

// TODO: what will happen if for example the user loaded one folder
// then exported while loading another folder?
bool ImageRepository::load_directory(const std::string& path) {
    try {
        const std::regex filter(IMAGE_FILE_REGEX);
        std::smatch what;

        std::vector<std::filesystem::path> image_paths;

        // Collect image file paths
        for (std::filesystem::recursive_directory_iterator it(path); it != std::filesystem::recursive_directory_iterator(); ++it) {
            if (!std::filesystem::is_regular_file(it->status()))
                continue;

            std::string filename = it->path().filename().string();
            boost::to_lower(filename);
            if (!std::regex_search(filename, what, filter))
                continue;

            image_paths.push_back(it->path());
        }

        // Return if no images found
        int images_count = image_paths.size();
        if (images_count == 0) return false;

        emit onImageLoadStarted(images_count);

        // Prepare for new data
        selectImage(-1);
        _images.clear();
        _folderPath = path;


        const size_t batch_size = 17;
        std::vector<std::future<std::vector<std::unique_ptr<Image>>> > futures;

        for (size_t i = 0; i < image_paths.size(); i += batch_size) {
            auto start = image_paths.begin() + i;
            auto end = (i + batch_size < image_paths.size()) ? start + batch_size : image_paths.end();

            std::vector<std::filesystem::path> batch(start, end);

            auto future = post(ThreadPool::instance(), use_future([this, batch]() {
                std::vector<std::unique_ptr<Image>> local_images;
                for (const auto& file : batch) {
                    auto image = std::make_unique<Image>();
                    image->load(file.string());
                    local_images.push_back(std::move(image));
                }
                return local_images;
            }));

            futures.push_back(std::move(future));
        }

        for (auto& future : futures) {
            auto local_images = future.get();
            for (auto& image : local_images) {
                _images.push_back(std::move(image));
                emit onImageLoaded(_images.back().get());
            }
        }

        emit onDirectoryChanged(path, getImages(), false);

        return true;
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    } catch (const std::regex_error& ex) {
        std::cerr << "Regex error: " << ex.what() << std::endl;
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
    emit onImageLoadStarted(1);

    std::unique_ptr<Image> img = std::make_unique<Image>();
    load_image_core(std::move(img), path, &_images);

    emit onDirectoryChanged(fpath.remove_filename().string(), getImages(), true);

    return true;
}

void ImageRepository::load_image_core(std::unique_ptr<Image> image, const std::string &path, std::vector<std::unique_ptr<Image>>* container) {
    image->load(path);

    container->push_back(std::move(image));

    emit onImageLoaded(image.get());
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

Image *ImageRepository::getImage(const std::size_t index)
{
    if (index < _images.size())
        return _images[index].get();

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
