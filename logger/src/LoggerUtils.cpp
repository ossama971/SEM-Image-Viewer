#include "LoggerUtils.h"

boost::atomic<uint8_t> MAX_LOG_LEVEL_FILTER{0};
Log *LOGGER = NopLogger::get_instance();
boost::atomic<uint8_t> STATE{UNINTIALIZED};

void set_max_level(Level level) {
    MAX_LOG_LEVEL_FILTER.store(static_cast<uint8_t>(level.get_typ()),
                               boost::memory_order_relaxed);
}

Level get_max_level() {
    return Level(static_cast<Level::LevelType>(
        MAX_LOG_LEVEL_FILTER.load(boost::memory_order_relaxed)));
}

std::pair<bool, const char *> set_logger(boost::function<Log *()> make_logger) {
    uint8_t expected = UNINTIALIZED;
    if (STATE.compare_exchange_strong(expected, INTIALIZING,
                                       boost::memory_order_acquire,
                                       boost::memory_order_relaxed)) {
        LOGGER = make_logger();
        STATE.store(INTIALIZED, boost::memory_order_release);
        return std::make_pair(true, "Logger set successfully");
    } else if (expected == INTIALIZING) {
        while (STATE.load(boost::memory_order_relaxed) == INTIALIZING) {
            boost::this_thread::yield();
        }
        return std::make_pair(false, SET_LOGGER_ERROR);
    } else {
        return std::make_pair(false, SET_LOGGER_ERROR);
    }
}

Log *get_logger() {
    if (STATE.load(boost::memory_order_relaxed) != INTIALIZED) {
        return NopLogger::get_instance();
    } else {
        return LOGGER;
    }
}

