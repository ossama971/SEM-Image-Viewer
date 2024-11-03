#ifndef BUFFER_WRITER_H
#define BUFFER_WRITER_H

#include <boost/shared_ptr.hpp>

#include "Buffer.h"
#include "Target.h"

class BufferWriter {
public:
  BufferWriter() = default;

  BufferWriter(boost::shared_ptr<WritableTarget> target) : target(target) {
  }

  // Copy constructor
  BufferWriter(const BufferWriter &other) : target(other.target) {
  }

  // Copy assignment operator
  BufferWriter &operator=(const BufferWriter &other) {
    if (this != &other) {
      target = other.target;
    }
    return *this;
  }

  // Move constructor
  BufferWriter(BufferWriter &&other) noexcept : target(std::move(other.target)) {
    std::cout << "BufferWriter move constructor" << std::endl;
  }

  // Move assignment operator
  BufferWriter &operator=(BufferWriter &&other) noexcept {
    if (this != &other) {
      target = std::move(other.target);
    }
    return *this;
  }

  void write(const Buffer &buffer) const {
    if (target) {
      target->write(buffer);
    } else {
      std::cerr << "Error: Target is nullptr in BufferWriter::write" << std::endl;
    }
  }

  void set_target(boost::shared_ptr<WritableTarget> target) {
    this->target = target;
  }

  Buffer get_buffer() const { return Buffer(); }

  boost::shared_ptr<WritableTarget> get_target() const { return target; }

  virtual ~BufferWriter() = default;

private:
  boost::shared_ptr<WritableTarget> target;
};

class Writer {
public:
  Writer() = default;
  Writer(const BufferWriter &buffer_writer) : buffer_writer(buffer_writer) {}
  
  Writer(const Writer &other) : buffer_writer(other.buffer_writer) {
  }
  Writer(Writer &&other) noexcept : buffer_writer(std::move(other.buffer_writer)) {}
  Writer &operator=(const Writer &other) {
    buffer_writer = other.buffer_writer;
    return *this;
  }
  Writer &operator=(Writer &&other) noexcept {
    buffer_writer = std::move(other.buffer_writer);
    return *this;
  }

  void print(const Buffer &buffer) const {
    buffer_writer.write(buffer);
  }

  Buffer get_buffer() const { 
    return buffer_writer.get_buffer(); 
  }

  boost::shared_ptr<WritableTarget> get_target() const { 
    return buffer_writer.get_target(); 
  }

  virtual ~Writer() = default;

private:
  BufferWriter buffer_writer;
};

#endif // BUFFER_WRITER_H
