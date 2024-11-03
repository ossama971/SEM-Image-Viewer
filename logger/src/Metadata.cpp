#include "Metadata.h"

MetadataBuilder Metadata::builder() {
    return MetadataBuilder();
}

Level Metadata::get_level() const {
    return level;
}

std::string Metadata::get_target() const {
    return target;
}

MetadataBuilder::MetadataBuilder() {
    metadata.level = Level(Level::LevelType::INFO);
    metadata.target = "";
}

MetadataBuilder& MetadataBuilder::set_level(Level _level) {
    metadata.level = _level;
    return *this;
}

MetadataBuilder& MetadataBuilder::set_target(const std::string &_target) {
    metadata.target = _target;
    return *this;
}

Metadata MetadataBuilder::build() const {
    return metadata;
}

