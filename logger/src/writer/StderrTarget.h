#ifndef STDERR_TARGET_H
#define STDERR_TARGET_H

#include <iostream>
#include <boost/thread/mutex.hpp>
#include "WritableTarget.h"

// A WritableTarget that writes output to the standard error stream.
class StderrTarget : public WritableTarget {
public:
    void write(const Buffer &buffer) const override;

private:
    mutable boost::mutex mutex;
};

#endif // STDERR_TARGET_H

