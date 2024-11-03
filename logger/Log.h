#ifndef LOG_H
#define LOG_H

#include "Record.h"
#include "Metadata.h"

class Log {
public:
  virtual bool enabled(const Metadata &metada) = 0;
  virtual void log(const Record &record) = 0;
  virtual void flush() = 0;
  virtual ~Log() = default;
};

#endif // LOG_H
