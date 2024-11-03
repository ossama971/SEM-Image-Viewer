#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

class Buffer {
public:
    Buffer() = default;
    Buffer(const std::vector<uint8_t> &data);
    Buffer(const Buffer &other);
    Buffer(Buffer &&other) noexcept;

    Buffer& operator=(const Buffer &other);
    Buffer& operator=(Buffer &&other) noexcept;

    virtual ~Buffer() = default;

    void clear();
    void write(const std::vector<uint8_t> &data);
    void flush();
    std::vector<uint8_t> as_bytes() const;

private:
    std::vector<uint8_t> buffer;  // TODO: Consider replacing with a ring buffer
};

#endif // BUFFER_H
