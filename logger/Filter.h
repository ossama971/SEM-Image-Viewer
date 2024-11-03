#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include <boost/regex.hpp>

#include "Level.h"
#include "Record.h"

class Directive {
public:
  boost::optional<std::string> name;
  Level level;

  Directive() = default;
  virtual ~Directive() = default;

  Directive(const std::string &_name, Level _level)
      : name(_name), level(_level) {}

  Directive(boost::none_t, Level _level) : name(boost::none), level(_level) {}
};

// Check whether a level and target are enabled by the set of directives.
bool enabled(const std::vector<Directive> &directives, Level level,
             const std::string &target) {
  // Search for the longest match, the vector is assumed to be pre-sorted.
  for (auto it = directives.rbegin(); it != directives.rend(); ++it) {
    if(it->name != boost::none && boost::iequals(target, *it->name)) {
      return level <= it->level;
    }
  }
  return false;
}

class FilterOp {
public:
  FilterOp(const std::string &pattern) : pat(pattern) {}
  virtual ~FilterOp() = default;

  bool is_match(const std::string &target) const {
    return boost::regex_search(target, pat);
  }

private:
  boost::regex pat;
};

class Filter {
public:
  Filter() = default;
  Filter(const std::vector<Directive> &directives,
         const boost::optional<FilterOp> &filter)
      : directives(directives), filter_(filter) {}
  virtual ~Filter() = default;

  // returns the maximum levelfilter that this filter is configured to output
  Level filter() const {
    Level max_level = Level(Level::LevelType::OFF);
    for (const auto &directive : directives) {
      if (directive.level > max_level) {
        max_level = directive.level;
      }
    }
    return max_level;
  }

  bool matches(const Record &record) const {
    if (!this->enabled(record.get_metadata())) {
      return false;
    }
    if (filter_ != boost::none) {
      return filter_->is_match(record.get_args().str());
    }
    return true;
  }

  bool enabled(const Metadata &metadata) const {
    return ::enabled(directives, metadata.get_level(), metadata.get_target());
  }

private:
  std::vector<Directive> directives;
  boost::optional<FilterOp> filter_;
};

class FilterBuilder {
public:
  FilterBuilder &add_directive(const Directive &directive) {
    for (auto &d : directives) {
      if (d.name == directive.name) {
        d.level = directive.level;
        return *this;
      }
    }
    directives.push_back(directive);
    return *this;
  }

  FilterBuilder &set_filter(const FilterOp &filter) {
    filter_ = filter;
    return *this;
  }

  Filter build() {
    if (directives.size() == 0) {
      std::cout << "empty" << std::endl;
      directives.push_back(Directive(boost::none, Level_TRACE));
    }
    sort(directives.begin(), directives.end(),
         [](const Directive &a, const Directive &b) {
           return a.name < b.name;
         });
    return Filter(directives, filter_);
  }

private:
  std::vector<Directive> directives;
  boost::optional<FilterOp> filter_;
};

#endif // FILTER_H
