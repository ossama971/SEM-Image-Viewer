#ifndef TARGET_H
#define TARGET_H

#include <fstream>
#include <iostream>
#include <string>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include "Buffer.h"

// this mainly acts as a marker interface for any object that can be written
// to any device
//
// TODO: consider using async/await?
class WritableTarget {
public:
  virtual void write(const Buffer &buffer) const = 0;
  virtual ~WritableTarget() = default;
};

class StdoutTarget : public WritableTarget {
public:
  void write(const Buffer &buffer) const override {
    boost::lock_guard<boost::mutex> lock(mutex);
    std::cout
        .write(reinterpret_cast<const char *>(buffer.as_bytes().data()),
               buffer.as_bytes().size())
        .flush();
  }

private:
  mutable boost::mutex mutex;
};

class StderrTarget : public WritableTarget {
public:
  void write(const Buffer &buffer) const override {
    boost::lock_guard<boost::mutex> lock(mutex);
    std::cerr
        .write(reinterpret_cast<const char *>(buffer.as_bytes().data()),
               buffer.as_bytes().size())
        .flush();
  }

private:
  mutable boost::mutex mutex;
};

class FileTarget : public WritableTarget {
public:
  FileTarget(const std::string &filename) : filename(filename) {}

  void write(const Buffer &buffer) const override {
    boost::lock_guard<boost::mutex> lock(mutex);
    std::ofstream file(filename, std::ios::app | std::ios::binary);
    file.write(reinterpret_cast<const char *>(buffer.as_bytes().data()),
               buffer.as_bytes().size())
        .flush();
    file.close();
  }

private:
  mutable boost::mutex mutex;
  std::string filename;
};


#endif // TARGET_H
