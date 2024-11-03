#ifndef LEVEL_H
#define LEVEL_H

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <string>
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

    constexpr bool operator==(const Level a) const { return typ == a.typ; }
    constexpr bool operator!=(const Level a) const { return typ != a.typ; }
    constexpr bool operator<(const Level a) const { return typ < a.typ; }
    constexpr bool operator>(const Level a) const { return typ > a.typ; }
    constexpr bool operator<=(const Level a) const { return typ <= a.typ; }
    constexpr bool operator>=(const Level a) const { return typ >= a.typ; }

    const char* as_str() const;

    static boost::optional<Level> from_str(const std::string &_level);
    static Level max_level();
    constexpr uint8_t get_typ() const { return static_cast<uint8_t>(typ); }

private:
    LevelType typ;
};

// Macro definitions for level constants
#define Level_OFF   Level(Level::LevelType::OFF)
#define Level_ERROR Level(Level::LevelType::ERROR)
#define Level_WARN  Level(Level::LevelType::WARN)
#define Level_INFO  Level(Level::LevelType::INFO)
#define Level_DEBUG Level(Level::LevelType::DEBUG)
#define Level_TRACE Level(Level::LevelType::TRACE)

#endif // LEVEL_H
