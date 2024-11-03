#include "Writer.h"

Writer::Writer(const BufferWriter &buffer_writer) : buffer_writer(buffer_writer) {}

Writer::Writer(const Writer &other) : buffer_writer(other.buffer_writer) {}

Writer::Writer(Writer &&other) noexcept : buffer_writer(std::move(other.buffer_writer)) {}

Writer& Writer::operator=(const Writer &other) {
    if (this != &other) {
        buffer_writer = other.buffer_writer;
    }
    return *this;
}

Writer& Writer::operator=(Writer &&other) noexcept {
    if (this != &other) {
        buffer_writer = std::move(other.buffer_writer);
    }
    return *this;
}

void Writer::print(const Buffer &buffer) const {
    buffer_writer.write(buffer);
}

Buffer Writer::get_buffer() const {
    return buffer_writer.get_buffer();
}

boost::shared_ptr<WritableTarget> Writer::get_target() const {
    return buffer_writer.get_target();
}

