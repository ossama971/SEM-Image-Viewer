#ifndef FORMATTER_H
#define FORMATTER_H

#include <vector>

#include <boost/format.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/optional.hpp>

#include "Buffer.h"
#include "Writer.h"
#include "Record.h"

class Formatter {
public:
  Formatter() {
    buffer.reset(new Buffer());
  }
  virtual ~Formatter() = default;

  Formatter(const Formatter &other) : buffer(other.buffer) {}
  Formatter(Formatter &&other) noexcept : buffer(std::move(other.buffer)) {}
  Formatter &operator=(const Formatter &other) {
    buffer = other.buffer;
    return *this;
  }
  Formatter &operator=(Formatter &&other) noexcept {
    buffer = std::move(other.buffer);
    return *this;
  }

  Formatter(const Writer &writer) {
    buffer.reset(new Buffer(writer.get_buffer()));
  }

  void print(const Writer &writer) {
    boost::shared_lock<boost::shared_mutex> lock(mutex);
    writer.print(*buffer);
  }

  void write(const std::vector<uint8_t> &value) {
    boost::unique_lock<boost::shared_mutex> lock(mutex);
    buffer->write(value);
  }

  void clear() {
    boost::unique_lock<boost::shared_mutex> lock(mutex);
    buffer->clear();
  }

  Buffer get_buffer() const {
    return *buffer;
  }

  friend class FormatterBuilder;

private:
  boost::shared_ptr<Buffer> buffer;
  boost::shared_mutex mutex;
};

class DefaultFormatter {
public:
  // XXX: should return a Result-like and all the write_* methods as well
  void write(const Record &record) {
    // write_timestamp();
    write_level(record);
    write_module_path(record);
    write_target(record);
    finish_header();
    write_args(record);
  }

  explicit DefaultFormatter() {
    write_header = false;
    // timestamp = true;
    module_path = true;
    target = true;
    level = true;
    // indent = boost::none;
    formatter = Formatter();
    suffix = "\n";
  }

  Buffer get_buffer() const {
    return formatter.get_buffer();
  }

  void clear() {
    write_header = false;
    formatter.clear();
  }

private:
  void write_header_value(const std::string &value) const {
    if(!write_header) {
      write_header = true;
      std::vector<uint8_t> bytes(value.size() + 1);
      bytes.push_back('[');
      bytes.insert(bytes.end(), value.begin(), value.end());
      formatter.write(bytes);
    } else {
      std::vector<uint8_t> bytes(value.size() + 1);
      bytes.push_back(' ');
      bytes.insert(bytes.end(), value.begin(), value.end());
      formatter.write(bytes);
    }
  }

  void write_level(const Record &record) const {
    if(level) {
      std::string formatted = (boost::format("%-5s") % record.get_metadata().get_level().as_str()).str();
      write_header_value(formatted);
    }
  }

  void write_module_path(const Record &record) const {
    if(module_path) {
      if(record.get_module_path() != boost::none) {
        std::string formatted = (boost::format("%s:%d") % record.get_module_path().get() % record.get_line().get()).str();
        write_header_value(formatted);
      }
    }
  }

  void write_target(const Record &record) const {
    if(target) {
      if(record.get_metadata().get_target() != "") {
        write_header_value(record.get_metadata().get_target());
      }
    }
  }

  void finish_header() const {
    if(write_header) {
      std::vector<uint8_t> bytes{']', ' '};
      formatter.write(bytes);
    }
  }

  void write_args(const Record &record) const {
    const std::string &args = record.get_args().str();
    std::vector<uint8_t> bytes(args.begin(), args.end());
    if(suffix != "") {
      bytes.insert(bytes.end(), suffix.begin(), suffix.end());
    }
    formatter.write(bytes);
  }

private:
  mutable bool write_header;
  // bool timestamp;
  bool module_path;
  bool target;
  bool level;
  // boost::optional<std::size_t> indent;
  mutable Formatter formatter;
  std::string suffix;
};

#endif // FORMATTER_H
