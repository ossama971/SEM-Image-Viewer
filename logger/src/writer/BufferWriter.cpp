#include "BufferWriter.h"
#include <iostream>

BufferWriter::BufferWriter(boost::shared_ptr<WritableTarget> target) : target(target) {}

BufferWriter::BufferWriter(const BufferWriter &other) : target(other.target) {}

BufferWriter& BufferWriter::operator=(const BufferWriter &other) {
    if (this != &other) {
        target = other.target;
    }
    return *this;
}

BufferWriter::BufferWriter(BufferWriter &&other) noexcept : target(std::move(other.target)) {
    std::cout << "BufferWriter move constructor" << std::endl;
}

BufferWriter& BufferWriter::operator=(BufferWriter &&other) noexcept {
    if (this != &other) {
        target = std::move(other.target);
    }
    return *this;
}

void BufferWriter::write(const Buffer &buffer) const {
    if (target) {
        target->write(buffer);
    } else {
        std::cerr << "Error: Target is nullptr in BufferWriter::write" << std::endl;
    }
}

void BufferWriter::set_target(boost::shared_ptr<WritableTarget> target) {
    this->target = target;
}

Buffer BufferWriter::get_buffer() const {
    return Buffer();
}

boost::shared_ptr<WritableTarget> BufferWriter::get_target() const {
    return target;
}

