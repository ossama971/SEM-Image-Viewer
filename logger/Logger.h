#ifndef LOGGER_H
#define LOGGER_H

#include "Filter.h"
#include "Formatter.h"
#include "Level.h"
#include "Log.h"
#include "Writer.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/tss.hpp>

class Logger : public Log {
public:
  bool enabled(const Metadata &metadata) override {
    return this->filter_.enabled(metadata);
  }

  void log(const Record &record) override {
    if (this->matches(record)) {
      // thread_local Formatter formatter(this->writer_);
      thread_local DefaultFormatter fmt;
      fmt.write(record);
      this->writer_.print(fmt.get_buffer());
      fmt.clear();
      // const auto result = (*this->formatter_)(formatter, record);
      // if (result.first) {
      //   formatter.print(this->writer_);
      // }
    }
  }

  void flush() override {}

  Level filter() const { return filter_.filter(); }

  bool matches(const Record &record) const { return filter_.matches(record); }

  static Logger *get_instance();

private:
  Writer writer_;
  Filter filter_;
  // FormatFn formatter_;

  static boost::mutex mtx;
  static Logger *instance;

  Logger() = default;
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  friend class LoggerBuilder;
};

boost::mutex Logger::mtx;
Logger *Logger::instance = nullptr;

Logger *Logger::get_instance() {
  if (!instance) {
    boost::unique_lock<boost::mutex> lock(mtx);
    if (!instance) {
      instance = new Logger();
    }
    lock.unlock(); // release lock
  }
  return instance;
}

class LoggerBuilder {
public:
  LoggerBuilder &set_writer(const Writer &writer) {
    writer_ = writer;
    return *this;
  }

  LoggerBuilder &set_filter(const Filter &filter) {
    filter_ = filter;
    return *this;
  }

  // LoggerBuilder &set_formatter(FormatFn formatter) {
  //   formatter_ = std::move(formatter);
  //   return *this;
  // }

  Logger *build() {
    Logger *logger = Logger::get_instance();
    logger->writer_ = writer_;
    logger->filter_ = filter_;
    // logger->formatter_ = std::move(formatter_);
    return logger;
  }

private:
  Writer writer_;
  Filter filter_;
  // FormatFn formatter_;
};

#endif // LOGGER_H
