#ifndef FORMATTER_H
#define FORMATTER_H

#include <vector>
#include <boost/format.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/optional.hpp>

#include "../writer/Buffer.h"
#include "../writer/Writer.h"
#include "../Record.h"

class Formatter {
public:
    Formatter();
    virtual ~Formatter() = default;

    Formatter(const Formatter &other);
    Formatter(Formatter &&other) noexcept;
    Formatter &operator=(const Formatter &other);
    Formatter &operator=(Formatter &&other) noexcept;

    Formatter(const Writer &writer);

    void print(const Writer &writer);
    void write(const std::vector<uint8_t> &value);
    void clear();
    Buffer get_buffer() const;

    friend class FormatterBuilder;

private:
    boost::shared_ptr<Buffer> buffer;
    boost::shared_mutex mutex;
};

#endif // FORMATTER_H
