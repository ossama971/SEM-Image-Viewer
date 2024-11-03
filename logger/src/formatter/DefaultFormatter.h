#ifndef DEFAULT_FORMATTER_H
#define DEFAULT_FORMATTER_H

#include <string>
#include <vector>

#include "Formatter.h"
#include "../Record.h"

class DefaultFormatter {
public:
    explicit DefaultFormatter();
    void write(const Record &record);
    Buffer get_buffer() const;
    void clear();

private:
    void write_header_value(const std::string &value) const;
    void write_level(const Record &record) const;
    void write_module_path(const Record &record) const;
    void write_target(const Record &record) const;
    void finish_header() const;
    void write_args(const Record &record) const;

private:
    mutable bool write_header;
    bool module_path;
    bool target;
    bool level;
    mutable Formatter formatter;
    std::string suffix;
};

#endif // DEFAULT_FORMATTER_H

