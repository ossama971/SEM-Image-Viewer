#ifndef NOP_LOGGER_H
#define NOP_LOGGER_H

#include <boost/thread/mutex.hpp>

#include "Log.h"

class NopLogger : public Log {
public:
  bool enabled(const Metadata &metadata) override { return false; }

  void log(const Record &record) override {}

  void flush() override {}

  static NopLogger *get_instance();

  virtual ~NopLogger() = default;

private:
  static boost::mutex mtx;
  static NopLogger *instance;

  NopLogger() = default;
  NopLogger(const NopLogger &) = delete;
  NopLogger &operator=(const NopLogger &) = delete;
};

boost::mutex NopLogger::mtx;
NopLogger *NopLogger::instance = nullptr;

NopLogger *NopLogger::get_instance() {
  if (!instance) {
    boost::unique_lock<boost::mutex> lock(mtx);
    if (!instance) {
      instance = new NopLogger();
    }
    lock.unlock(); // release lock
  }
  return instance;
}

#endif // NOP_LOGGER_H
