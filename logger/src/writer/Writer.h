#ifndef WRITER_H
#define WRITER_H

#include "BufferWriter.h"

class Writer {
public:
    Writer() = default;
    explicit Writer(const BufferWriter &buffer_writer);

    // Copy constructor
    Writer(const Writer &other);

    // Move constructor
    Writer(Writer &&other) noexcept;

    // Copy assignment operator
    Writer& operator=(const Writer &other);

    // Move assignment operator
    Writer& operator=(Writer &&other) noexcept;

    void print(const Buffer &buffer) const;
    Buffer get_buffer() const;
    boost::shared_ptr<WritableTarget> get_target() const;

    virtual ~Writer() = default;

private:
    BufferWriter buffer_writer;
};

#endif // WRITER_H
