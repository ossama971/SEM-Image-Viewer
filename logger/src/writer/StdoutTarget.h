#ifndef STDOUT_TARGET_H
#define STDOUT_TARGET_H

#include <iostream>
#include <boost/thread/mutex.hpp>
#include "WritableTarget.h"

// A WritableTarget that writes output to the standard output stream.
class StdoutTarget : public WritableTarget {
public:
    void write(const Buffer &buffer) const override;

private:
    mutable boost::mutex mutex;
};

#endif // STDOUT_TARGET_H

