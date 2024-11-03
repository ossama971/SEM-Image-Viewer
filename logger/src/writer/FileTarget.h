#ifndef FILE_TARGET_H
#define FILE_TARGET_H

#include <fstream>
#include <string>
#include <boost/thread/mutex.hpp>
#include "WritableTarget.h"

// A WritableTarget that writes output to a specified file.
class FileTarget : public WritableTarget {
public:
    explicit FileTarget(const std::string &filename);
    void write(const Buffer &buffer) const override;

private:
    mutable boost::mutex mutex;
    std::string filename;
};

#endif // FILE_TARGET_H

