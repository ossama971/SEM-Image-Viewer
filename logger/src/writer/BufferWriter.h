#ifndef BUFFER_WRITER_H
#define BUFFER_WRITER_H

#include <boost/shared_ptr.hpp>
#include "Buffer.h"
#include "WritableTarget.h"

class BufferWriter {
public:
    BufferWriter() = default;
    explicit BufferWriter(boost::shared_ptr<WritableTarget> target);

    // Copy constructor
    BufferWriter(const BufferWriter &other);

    // Copy assignment operator
    BufferWriter& operator=(const BufferWriter &other);

    // Move constructor
    BufferWriter(BufferWriter &&other) noexcept;

    // Move assignment operator
    BufferWriter& operator=(BufferWriter &&other) noexcept;

    void write(const Buffer &buffer) const;
    void set_target(boost::shared_ptr<WritableTarget> target);
    Buffer get_buffer() const;
    boost::shared_ptr<WritableTarget> get_target() const;

    virtual ~BufferWriter() = default;

private:
    boost::shared_ptr<WritableTarget> target;
};

#endif // BUFFER_WRITER_H

