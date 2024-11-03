#include "DefaultFormatter.h"

DefaultFormatter::DefaultFormatter() {
    write_header = false;
    module_path = true;
    target = true;
    level = true;
    formatter = Formatter();
    suffix = "\n";
}

void DefaultFormatter::write(const Record &record) {
    write_level(record);
    write_module_path(record);
    write_target(record);
    finish_header();
    write_args(record);
}

Buffer DefaultFormatter::get_buffer() const {
    return formatter.get_buffer();
}

void DefaultFormatter::clear() {
    write_header = false;
    formatter.clear();
}

void DefaultFormatter::write_header_value(const std::string &value) const {
    if (!write_header) {
        write_header = true;
        std::vector<uint8_t> bytes(value.size() + 1);
        bytes.push_back('[');
        bytes.insert(bytes.end(), value.begin(), value.end());
        formatter.write(bytes);
    } else {
        std::vector<uint8_t> bytes(value.size() + 1);
        bytes.push_back(' ');
        bytes.insert(bytes.end(), value.begin(), value.end());
        formatter.write(bytes);
    }
}

void DefaultFormatter::write_level(const Record &record) const {
    if (level) {
        std::string formatted = (boost::format("%-5s") % record.get_metadata().get_level().as_str()).str();
        write_header_value(formatted);
    }
}

void DefaultFormatter::write_module_path(const Record &record) const {
    if (module_path) {
        if (record.get_module_path() != boost::none) {
            std::string formatted = (boost::format("%s:%d") % record.get_module_path().get() % record.get_line().get()).str();
            write_header_value(formatted);
        }
    }
}

void DefaultFormatter::write_target(const Record &record) const {
    if (target) {
        if (record.get_metadata().get_target() != "") {
            write_header_value(record.get_metadata().get_target());
        }
    }
}

void DefaultFormatter::finish_header() const {
    if (write_header) {
        std::vector<uint8_t> bytes{']', ' '};
        formatter.write(bytes);
    }
}

void DefaultFormatter::write_args(const Record &record) const {
    const std::string &args = record.get_args().str();
    std::vector<uint8_t> bytes(args.begin(), args.end());
    if (suffix != "") {
        bytes.insert(bytes.end(), suffix.begin(), suffix.end());
    }
    formatter.write(bytes);
}

