#include "FileTarget.h"

FileTarget::FileTarget(const std::string &filename) : filename(filename) {}

void FileTarget::write(const Buffer &buffer) const {
    boost::lock_guard<boost::mutex> lock(mutex);
    std::ofstream file(filename, std::ios::app | std::ios::binary);
    file.write(reinterpret_cast<const char *>(buffer.as_bytes().data()),
               buffer.as_bytes().size()).flush();
}

