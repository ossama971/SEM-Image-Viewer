#ifndef NOP_LOGGER_H
#define NOP_LOGGER_H

#include <boost/thread/mutex.hpp>
#include "Log.h"

class NopLogger : public Log {
public:
    static NopLogger* get_instance();

    bool enabled(const Metadata &metadata) override;
    void log(const Record &record) override;
    void flush() override;

    virtual ~NopLogger() = default;

private:
    NopLogger() = default; // Private constructor for singleton
    NopLogger(const NopLogger &) = delete; // Disable copy constructor
    NopLogger &operator=(const NopLogger &) = delete; // Disable copy assignment

    static boost::mutex mtx; // Mutex for thread safety
    static NopLogger *instance; // Singleton instance
};

#endif // NOP_LOGGER_H
