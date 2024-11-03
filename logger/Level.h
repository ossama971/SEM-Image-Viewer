#ifndef LEVEL_H
#define LEVEL_H

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

#include "config.h"

class Level {
public:
  enum class LevelType : uint8_t {
    OFF = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
    DEBUG = 4,
    TRACE = 5
  };

  Level() = default;
  constexpr explicit Level(LevelType _typ) : typ(_typ) {}

  constexpr bool operator==(Level a) const { return typ == a.typ; }
  constexpr bool operator!=(Level a) const { return typ != a.typ; }
  constexpr bool operator<(Level a) const { return typ < a.typ; }
  constexpr bool operator>(Level a) const { return typ > a.typ; }
  constexpr bool operator<=(Level a) const { return typ <= a.typ; }
  constexpr bool operator>=(Level a) const { return typ >= a.typ; }

  inline const char *as_str() const {
    return LOG_LEVELS_NAMES[static_cast<uint8_t>(typ)];
  }

  static boost::optional<Level> from_str(const std::string &_level) {
    boost::optional<Level> result = boost::none;
    std::string level = _level;
    boost::trim(level);
    for (uint8_t i = 0; i < 6; ++i) {
      if (boost::iequals(level, LOG_LEVELS_NAMES[i])) {
        result = Level(static_cast<enum LevelType>(i));
        break;
      }
    }
    return result;
  }

  static inline Level max_level() { return Level(LevelType::TRACE); }

  constexpr uint8_t get_typ() const { return static_cast<uint8_t>(typ); }

private:
  LevelType typ;
};

#define Level_OFF   Level(Level::LevelType::OFF)
#define Level_ERROR Level(Level::LevelType::ERROR)
#define Level_WARN  Level(Level::LevelType::WARN)
#define Level_INFO  Level(Level::LevelType::INFO)
#define Level_DEBUG Level(Level::LevelType::DEBUG)
#define Level_TRACE Level(Level::LevelType::TRACE)

#endif // LEVEL_H
