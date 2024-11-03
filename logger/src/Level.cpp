#include "Level.h"

const char* Level::as_str() const {
    return LOG_LEVELS_NAMES[static_cast<uint8_t>(typ)];
}

boost::optional<Level> Level::from_str(const std::string &_level) {
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

Level Level::max_level() {
    return Level(LevelType::TRACE);
}
