#ifndef RECORD_H
#define RECORD_H

#include <boost/format.hpp>
#include <boost/optional.hpp>

#include "Metadata.h"

class RecordBuilder;

class Record {
public:
  static RecordBuilder builder();

  inline Metadata get_metadata() const { return metadata; }

  inline boost::format get_args() const { return args; }

  inline boost::optional<std::string> get_module_path() const {
    return module_path;
  }

  inline boost::optional<std::string> get_file() const { return file; }

  inline boost::optional<uint32_t> get_line() const { return line; }

  virtual ~Record() = default;

  friend class RecordBuilder;

private:
  Record() = default;

  Metadata metadata;
  boost::format args;
  boost::optional<std::string> module_path;
  boost::optional<std::string> file;
  boost::optional<uint32_t> line;
};

class RecordBuilder {
public:
  inline RecordBuilder() {
    record.metadata = Metadata::builder().build();
    record.args = boost::format("");
    record.module_path = boost::none;
    record.file = boost::none;
    record.line = boost::none;
  }

  inline RecordBuilder &set_metadata(Metadata _metadata) {
    record.metadata = _metadata;
    return *this;
  }

  inline RecordBuilder &set_args(boost::format _args) {
    record.args = _args;
    return *this;
  }

  inline RecordBuilder &set_module_path(const std::string &_module_path) {
    record.module_path = _module_path;
    return *this;
  }

  inline RecordBuilder &set_file(const std::string &_file) {
    record.file = _file;
    return *this;
  }

  inline RecordBuilder &set_line(uint32_t _line) {
    record.line = _line;
    return *this;
  }

  inline Record build() const { return record; }

  virtual ~RecordBuilder() = default;

private:
  Record record;
};

RecordBuilder Record::builder() { return RecordBuilder(); }

#endif // RECORD_H
