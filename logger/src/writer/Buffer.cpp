#include "Buffer.h"

Buffer::Buffer(const std::vector<uint8_t> &data) : buffer(data) {}

Buffer::Buffer(const Buffer &other) : buffer(other.buffer) {}

Buffer::Buffer(Buffer &&other) noexcept : buffer(std::move(other.buffer)) {}

Buffer& Buffer::operator=(const Buffer &other) {
    if (this != &other) {
        buffer = other.buffer;
    }
    return *this;
}

Buffer& Buffer::operator=(Buffer &&other) noexcept {
    if (this != &other) {
        buffer = std::move(other.buffer);
    }
    return *this;
}

void Buffer::clear() {
    buffer.clear();
}

void Buffer::write(const std::vector<uint8_t> &data) {
    buffer.insert(buffer.end(), data.begin(), data.end());
}

void Buffer::flush() {}

std::vector<uint8_t> Buffer::as_bytes() const {
    return buffer;
}

