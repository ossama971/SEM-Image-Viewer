#ifndef METADATA_H
#define METADATA_H

#include <string>
#include "Level.h"

class MetadataBuilder;

class Metadata {
public:
    static MetadataBuilder builder();

    Level get_level() const;
    std::string get_target() const;

    virtual ~Metadata() = default;

private:
    Metadata() = default;

    Level level;
    std::string target;

    friend class MetadataBuilder;
    friend class Record;
};

class MetadataBuilder {
public:
    MetadataBuilder();

    MetadataBuilder& set_level(Level _level);
    MetadataBuilder& set_target(const std::string &_target);

    Metadata build() const;

    virtual ~MetadataBuilder() = default;

private:
    Metadata metadata;
};

#endif // METADATA_H
