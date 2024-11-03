#ifndef CONFIG_H
#define CONFIG_H

constexpr char const *LOG_LEVELS_NAMES[6] = {"OFF\0",  "ERROR\0", "WARN\0",
                                             "INFO\0", "DEBUG\0", "TRACE\0"};
constexpr char const *SET_LOGGER_ERROR =
    "attempted to set a logger after the logging system was already "
    "initialized\0";

constexpr const char *LEVEL_PARSE_ERROR =
    "attempted to convert a string that doesn't match an existing log level\0";


#endif // CONFIG_H
