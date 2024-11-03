#ifndef METADATA_H
#define METADATA_H

#include <string>

#include "Level.h"

class MetadataBuilder;

class Metadata {
public:
  static MetadataBuilder builder();

  inline Level get_level() const { return level; }

  inline std::string get_target() const { return target; }

  virtual ~Metadata() = default;

  friend class MetadataBuilder;
  friend class Record;

private:
  Metadata() = default;

  Level level;
  std::string target;
};

class MetadataBuilder {
public:
  inline MetadataBuilder() {
    metadata.level = Level(Level::LevelType::INFO);
    metadata.target = "";
  }

  inline MetadataBuilder &set_level(Level _level) {
    metadata.level = _level;
    return *this;
  }

  inline MetadataBuilder &set_target(const std::string &_target) {
    metadata.target = _target;
    return *this;
  }

  inline Metadata build() const { return metadata; }

  virtual ~MetadataBuilder() = default;

private:
  Metadata metadata;
};

MetadataBuilder Metadata::builder() { return MetadataBuilder(); }

#endif // METADATA_H
