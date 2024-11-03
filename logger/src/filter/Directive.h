#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include <string>
#include <boost/optional.hpp>
#include "../Level.h"

class Directive {
public:
    boost::optional<std::string> name;
    Level level;

    Directive() = default;
    Directive(const std::string &_name, Level _level) : name(_name), level(_level) {}
    Directive(boost::none_t, Level _level) : name(boost::none), level(_level) {}
    virtual ~Directive() = default;
};

#endif // DIRECTIVE_H

