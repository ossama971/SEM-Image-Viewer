#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

class Buffer {
public:
  Buffer() = default;
  virtual ~Buffer() = default;

  Buffer(const std::vector<uint8_t> &data) : buffer(data) {}
  Buffer(const Buffer &other) : buffer(other.buffer) {}
  Buffer(Buffer &&other) noexcept : buffer(std::move(other.buffer)) {}

  Buffer &operator=(const Buffer &other) {
    buffer = other.buffer;
    return *this;
  }
  Buffer &operator=(Buffer &&other) noexcept {
    buffer = std::move(other.buffer);
    return *this;
  }

  void clear() { buffer.clear(); }

  void write(const std::vector<uint8_t> &data) {
    buffer.insert(buffer.end(), data.begin(), data.end());
  }

  void flush() {}

  std::vector<uint8_t> as_bytes() const { return buffer; }

private:
  // XXX: should be replaced by a ring buffer
  std::vector<uint8_t> buffer;
};

#endif // BUFFER_H
