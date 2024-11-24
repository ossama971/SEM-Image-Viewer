#include "../engines/ThreadPool.h"
#include "ImageRepository.h"

#include <QDebug>
#include <QVector>
#include <filesystem>
#include <regex>

#include "../engines/Logger.h"
#include <boost/algorithm/string.hpp>

ImageRepository::ImageRepository() : _selectedImage(nullptr) {}

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
        Logger::MessageTypes::INFO, Logger::MessageID::IMAGES_LOADING_STARTED,
        Logger::MessageOptian::WITH_DETAILS_AND_PATH,
        {QString::fromStdString(path)}, image_paths.size(), "Loading .... ",
        QString("file:///%1").arg(QString::fromStdString(path)));

    // Prepare for new data
    _images.clear();
    _folderPath = path;
    _selectedImage = nullptr;

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
              auto image = std::make_unique<Image>();
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

    emit onDirectoryChanged(path, getImages(), false);
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

  _images.clear();
  _selectedImage = nullptr;

  std::unique_ptr<Image> img = std::make_unique<Image>();
  img->load(path);
  _images.push_back(std::move(img));

  emit onDirectoryChanged(fpath.parent_path().string(), getImages(), true);
  return true;
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

std::vector<Image *> ImageRepository::getImages() const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  std::vector<Image *> images;
  for (const auto &image : _images) {
    images.push_back(image.get());
  }
  return images;
}

std::vector<Image *>
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

std::string ImageRepository::getFolderPath() const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  return _folderPath;
}

void ImageRepository::accept(Visitor &v) const {
  std::unique_lock<std::recursive_mutex> lock(_mutex);
  v.visit(*this);
}
