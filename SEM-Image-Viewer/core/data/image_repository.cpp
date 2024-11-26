#include "image_repository.h"
#include "../engines/thread_pool.h"
#include "../engines/logger.h"

#include <QDebug>
#include <QVector>
#include <regex>
#include <boost/algorithm/string.hpp>

#ifndef IMAGE_CACHE
#include "image_cacheless.h"
#endif

ImageRepository::ImageRepository() : _selectedImage(nullptr)
#ifdef IMAGE_CACHE
    ,_cachePool(IMAGE_CACHE_SIZE)
#endif
{
#ifdef IMAGE_CACHE
    connect(&_cachePool, &ImageCachePool::onImageLoaded, this, &ImageRepository::onCacheImageLoaded);
    connect(&_cachePool, &ImageCachePool::onImageRemoved, this, &ImageRepository::onCacheImageRemoved);
#endif
}

bool ImageRepository::load_directory(const std::string &path) {
  std::unique_lock<std::recursive_mutex> lock(_mutex);

  try {
    const std::regex filter(IMAGE_FILE_REGEX);
    std::smatch what;
    std::vector<std::filesystem::path> image_paths;

    // Collect image file paths
    for (std::filesystem::recursive_directory_iterator it(path);
         it != std::filesystem::recursive_directory_iterator(); ++it) {
      if (!std::filesystem::is_regular_file(it->status()))
        continue;

      std::string filename = it->path().filename().string();
      boost::to_lower(filename);
      if (!std::regex_search(filename, what, filter))
        continue;

      image_paths.push_back(it->path());
    }

    if (image_paths.empty())
      return false;

    int progressbarID = Logger::instance()->logMessageWithProgressBar(
        Logger::MessageTypes::info, Logger::MessageID::images_loading_started,
        Logger::MessageOption::with_path,
        {QString::fromStdString(path)}, image_paths.size(),
        QString("file:///%1").arg(QString::fromStdString(path)));

    // Prepare for new data
    pre_directory_change(image_paths.size(), path);

    lock.unlock(); // Unlock mutex while processing images in threads

    const size_t batch_size = 17;
    std::vector<std::future<std::vector<std::unique_ptr<Image>>>> futures;

    for (size_t i = 0; i < image_paths.size(); i += batch_size) {
      auto start = image_paths.begin() + i;
      auto end = (i + batch_size < image_paths.size()) ? start + batch_size
                                                       : image_paths.end();
      std::vector<std::filesystem::path> batch(start, end);

      auto future = post(
          ThreadPool::instance(), use_future([this, progressbarID, batch]() {
            std::vector<std::unique_ptr<Image>> local_images;
            for (const auto &file : batch) {
#ifdef IMAGE_CACHE
                std::unique_ptr<Image> image = std::make_unique<Image>(&_cachePool);
#else
                std::unique_ptr<Image> image = std::make_unique<ImageCacheless>();
#endif
              image->load(file.string());
              local_images.push_back(std::move(image));
              Logger::instance()->updateProgressBar(progressbarID, 1);
            }
            return local_images;
          }));

      futures.push_back(std::move(future));
    }

    for (auto &future : futures) {
      auto local_images = future.get();

      lock.lock(); // Lock again to modify shared state
      for (auto &image : local_images) {
        _images.push_back(std::move(image));
      }
      lock.unlock(); // Unlock after modifying
    }

    post_directory_change(path, false);
    return true;
  } catch (const std::filesystem::filesystem_error &ex) {
    std::cerr << "Filesystem error: " << ex.what() << std::endl;
  } catch (const std::regex_error &ex) {
    std::cerr << "Regex error: " << ex.what() << std::endl;
  }
  return false;
}

bool ImageRepository::load_image(const std::string &path) {
    std::unique_lock<std::recursive_mutex> lock(_mutex);

    std::filesystem::path fpath(path);
    if (!std::filesystem::exists(fpath))
        return false;

    const std::regex filter(IMAGE_FILE_REGEX);
    std::smatch what;

    if (!std::regex_search(path, what, filter))
        return false;

    pre_directory_change(1, fpath.remove_filename().string());

#ifdef IMAGE_CACHE
    std::unique_ptr<Image> img = std::make_unique<Image>(&_cachePool);
#else
    std::unique_ptr<ImageCacheless> img = std::make_unique<ImageCacheless>();
#endif

    img->load(path);
    _images.push_back(std::move(img));

    post_directory_change(fpath.parent_path().string(), true);
    return true;
}

void ImageRepository::pre_directory_change(int image_count, const std::string &dir) {
    selectImage(-1);

    _folderPath = dir;
    _hasUnsavedChanges = false;
    _images.clear();

#ifdef IMAGE_CACHE
    _cachePool.clear();

    std::string cacheDir = std::filesystem::current_path().string() + "/" + IMAGE_CACHE_DIR;
    if (!std::filesystem::exists(cacheDir))
        std::filesystem::create_directory(cacheDir);
    else
    {
        for (auto& cacheItem : std::filesystem::directory_iterator(cacheDir))
            std::filesystem::remove(cacheItem.path());
    }
#endif

    //emit onImageLoadStarted(image_count);
}

void ImageRepository::post_directory_change(const std::string &newDir, bool image_load) {
    for (const auto& image : _images) {
        connect(image.get(), &Image::onImageStateUpdated, this, &ImageRepository::setUnsavedChanges);
    }

    emit onDirectoryChanged(newDir, getImages(), image_load);

    if (!_images.empty())
        selectImage(0);
}

void ImageRepository::selectImage(int index) {
  std::unique_lock<std::recursive_mutex> lock(_mutex);

  if (index == -1) {
    _selectedImage = nullptr;
  } else if (index < _images.size()) {
    _selectedImage = _images[index].get();
  }

  Image *selected = _selectedImage; // Snapshot for signal emission
  lock.unlock();

  emit onImageChanged(selected);
  if (selected) {
    emit loadActionList(selected->getHistory());
  }
}

void ImageRepository::selectImage(const std::string &path) {
  std::unique_lock<std::recursive_mutex> lock(_mutex);

    auto it = std::find_if(_images.begin(), _images.end(), [&path](const std::unique_ptr<Image> &image) {
        return image->getPath() == path;
    });

    if (it != _images.end()) {
        _selectedImage = it->get();
    } else {
        _selectedImage = nullptr;
    }

    Image* selected = _selectedImage;
    lock.unlock();

    emit onImageChanged(selected);
    if (selected) {
        emit loadActionList(selected->getHistory());
    }
}

void ImageRepository::onCacheImageLoaded(const std::string &path, QImage *image, cv::Mat* imageMat) {
    std::unique_lock<std::recursive_mutex> lock(_mutex);

    for (auto it = _images.begin(); it != _images.end(); ++it)
        (*it)->onCacheImageLoaded(path, image, imageMat);
}

void ImageRepository::onCacheImageRemoved(const std::string &path, QImage *image, cv::Mat* imageMat) {
    if (!std::filesystem::exists(path))
        cv::imwrite(path, *imageMat);
}

std::size_t ImageRepository::getImagesCount() const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  return _images.size();
}

Image *ImageRepository::getImage() {
  std::scoped_lock lock(_mutex);
  return _selectedImage;
}

Image *ImageRepository::getImage(const std::filesystem::path &path) {
  std::scoped_lock lock(_mutex);
  for (auto it = _images.begin(); it != _images.end(); ++it) {
    if (!(*it)->getPath().compare(path))
      return it->get();
  }

  return nullptr;
}

Image *ImageRepository::getImage(const std::size_t index) {
  std::scoped_lock lock(_mutex);
  if (index < _images.size())
    return _images[index].get();

  return nullptr;
}

const std::vector<Image *> ImageRepository::getImages() const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  std::vector<Image *> images;
  for (const auto &image : _images) {
    images.push_back(image.get());
  }
  return images;
}

const std::vector<Image *>
ImageRepository::getImages(const std::vector<int> &indices) const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  std::vector<Image *> result;

  for (int index : indices) {
    if (index >= 0 && index < _images.size()) {
      result.push_back(_images[index].get());
    }
  }
  return result;
}

const std::string ImageRepository::getFolderPath() const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  return _folderPath;
}

void ImageRepository::accept(Visitor &v) const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  v.visit(*this);
}

void ImageRepository::setUnsavedChanges(Image* image) {
    if (image->isChanged())
        _hasUnsavedChanges = true; // Set to true when any image state changes
}

bool ImageRepository::getHasUnsavedChanges(){
    return _hasUnsavedChanges;
}

std::vector<std::unique_ptr<Image>> ImageRepository::cloneImages() const {
    std::unique_lock<std::recursive_mutex> lock(_mutex);
    std::vector<std::unique_ptr<Image>> clonedImages;
    for (const auto& image : _images) {
        clonedImages.push_back(std::make_unique<Image>(*image));
    }
    return clonedImages;
}

std::unique_ptr<Image> ImageRepository::cloneSelectedImage() const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  if (_selectedImage)
    return std::make_unique<Image>(*_selectedImage);
  return nullptr;
}
