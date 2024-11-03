#ifndef LOGGER_H
#define LOGGER_H

#include "Level.h"
#include "Log.h"
#include "filter/Filter.h"
#include "formatter/DefaultFormatter.h"
#include "formatter/Formatter.h"
#include "writer/Writer.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/tss.hpp>

class Logger : public Log {
public:
    static Logger* get_instance();

    bool enabled(const Metadata &metadata) override;
    void log(const Record &record) override;
    void flush() override;

    Level filter() const;
    bool matches(const Record &record) const;

private:
    Logger() = default; // Private constructor for singleton
    Logger(const Logger &) = delete; // Disable copy constructor
    Logger &operator=(const Logger &) = delete; // Disable copy assignment

    Writer writer_;
    Filter filter_;

    static boost::mutex mtx; // Mutex for thread safety
    static Logger *instance; // Singleton instance

    friend class LoggerBuilder;
};

class LoggerBuilder {
public:
    LoggerBuilder &set_writer(const Writer &writer);
    LoggerBuilder &set_filter(const Filter &filter);
    Logger *build();

private:
    Writer writer_;
    Filter filter_;
};

#endif // LOGGER_H
