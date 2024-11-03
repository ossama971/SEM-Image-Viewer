#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <utility>

#include "config.h"
#include "Log.h"
#include "NopLogger.h"
#include "Level.h"

constexpr uint8_t UNINTIALIZED = 0;
constexpr uint8_t INTIALIZING = 1;
constexpr uint8_t INTIALIZED = 2;

// XXX: rethink these globals, for thread safety
extern boost::atomic<uint8_t> MAX_LOG_LEVEL_FILTER;
extern Log *LOGGER;
extern boost::atomic<uint8_t> STATE;

void set_max_level(Level level);
Level get_max_level();
std::pair<bool, const char *> set_logger(boost::function<Log *()> make_logger);
Log *get_logger();

#endif // LOGGER_UTILS_H

