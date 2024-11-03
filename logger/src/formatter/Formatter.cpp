#include "Formatter.h"

Formatter::Formatter() {
    buffer.reset(new Buffer());
}

Formatter::Formatter(const Formatter &other) : buffer(other.buffer) {}

Formatter::Formatter(Formatter &&other) noexcept : buffer(std::move(other.buffer)) {}

Formatter &Formatter::operator=(const Formatter &other) {
    buffer = other.buffer;
    return *this;
}

Formatter &Formatter::operator=(Formatter &&other) noexcept {
    buffer = std::move(other.buffer);
    return *this;
}

Formatter::Formatter(const Writer &writer) {
    buffer.reset(new Buffer(writer.get_buffer()));
}

void Formatter::print(const Writer &writer) {
    boost::shared_lock<boost::shared_mutex> lock(mutex);
    writer.print(*buffer);
}

void Formatter::write(const std::vector<uint8_t> &value) {
    boost::unique_lock<boost::shared_mutex> lock(mutex);
    buffer->write(value);
}

void Formatter::clear() {
    boost::unique_lock<boost::shared_mutex> lock(mutex);
    buffer->clear();
}

Buffer Formatter::get_buffer() const {
    return *buffer;
}

