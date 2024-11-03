#include "Record.h"

RecordBuilder Record::builder() {
    return RecordBuilder();
}

Metadata Record::get_metadata() const {
    return metadata;
}

boost::format Record::get_args() const {
    return args;
}

boost::optional<std::string> Record::get_module_path() const {
    return module_path;
}

boost::optional<std::string> Record::get_file() const {
    return file;
}

boost::optional<uint32_t> Record::get_line() const {
    return line;
}

// RecordBuilder class implementations

RecordBuilder::RecordBuilder() {
    record.metadata = Metadata::builder().build();
    record.args = boost::format("");
    record.module_path = boost::none;
    record.file = boost::none;
    record.line = boost::none;
}

RecordBuilder& RecordBuilder::set_metadata(Metadata _metadata) {
    record.metadata = _metadata;
    return *this;
}

RecordBuilder& RecordBuilder::set_args(boost::format _args) {
    record.args = _args;
    return *this;
}

RecordBuilder& RecordBuilder::set_module_path(const std::string &_module_path) {
    record.module_path = _module_path;
    return *this;
}

RecordBuilder& RecordBuilder::set_file(const std::string &_file) {
    record.file = _file;
    return *this;
}

RecordBuilder& RecordBuilder::set_line(uint32_t _line) {
    record.line = _line;
    return *this;
}

Record RecordBuilder::build() const {
    return record;
}

