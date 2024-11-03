#ifndef RECORD_H
#define RECORD_H

#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <string>
#include "Metadata.h"

class RecordBuilder;

class Record {
public:
    static RecordBuilder builder();

    Metadata get_metadata() const;
    boost::format get_args() const;
    boost::optional<std::string> get_module_path() const;
    boost::optional<std::string> get_file() const;
    boost::optional<uint32_t> get_line() const;

    virtual ~Record() = default;

private:
    Record() = default;

    Metadata metadata;
    boost::format args;
    boost::optional<std::string> module_path;
    boost::optional<std::string> file;
    boost::optional<uint32_t> line;

    friend class RecordBuilder;
};

class RecordBuilder {
public:
    RecordBuilder();

    RecordBuilder& set_metadata(Metadata _metadata);
    RecordBuilder& set_args(boost::format _args);
    RecordBuilder& set_module_path(const std::string &_module_path);
    RecordBuilder& set_file(const std::string &_file);
    RecordBuilder& set_line(uint32_t _line);

    Record build() const;

    virtual ~RecordBuilder() = default;

private:
    Record record;
};

#endif // RECORD_H
