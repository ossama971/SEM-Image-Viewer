#include "StdoutTarget.h"

void StdoutTarget::write(const Buffer &buffer) const {
    boost::lock_guard<boost::mutex> lock(mutex);
    std::cout.write(reinterpret_cast<const char *>(buffer.as_bytes().data()),
                    buffer.as_bytes().size()).flush();
}

