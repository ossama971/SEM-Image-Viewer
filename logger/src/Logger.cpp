#include "Logger.h"

boost::mutex Logger::mtx;
Logger* Logger::instance = nullptr;

Logger* Logger::get_instance() {
    if (!instance) {
        boost::unique_lock<boost::mutex> lock(mtx);
        if (!instance) {
            instance = new Logger();
        }
        // mtx.unlock();
    }
    return instance;
}

bool Logger::enabled(const Metadata &metadata) {
    return this->filter_.enabled(metadata);
}

void Logger::log(const Record &record) {
    if (this->matches(record)) {
        thread_local DefaultFormatter fmt;
        fmt.write(record);
        this->writer_.print(fmt.get_buffer());
        fmt.clear();
    }
}

void Logger::flush() {
    // Implement flushing logic if necessary
}

Level Logger::filter() const {
    return filter_.filter();
}

bool Logger::matches(const Record &record) const {
    return filter_.matches(record);
}

LoggerBuilder &LoggerBuilder::set_writer(const Writer &writer) {
    writer_ = writer;
    return *this;
}

LoggerBuilder &LoggerBuilder::set_filter(const Filter &filter) {
    filter_ = filter;
    return *this;
}

Logger *LoggerBuilder::build() {
    Logger *logger = Logger::get_instance();
    logger->writer_ = writer_;
    logger->filter_ = filter_;
    return logger;
}

