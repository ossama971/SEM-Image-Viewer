#include "Filter.h"
#include <boost/algorithm/string/predicate.hpp>

bool enabled(const std::vector<Directive> &directives, Level level, const std::string &target) {
    for (auto it = directives.rbegin(); it != directives.rend(); ++it) {
        if (it->name && boost::iequals(target, *it->name)) {
            return level <= it->level;
        }
    }
    return false;
}

Filter::Filter(const std::vector<Directive> &directives, const boost::optional<FilterOp> &filter)
    : directives(directives), filter_(filter) {}

Level Filter::filter() const {
    Level max_level = Level(Level::LevelType::OFF);
    for (const auto &directive : directives) {
        if (directive.level > max_level) {
            max_level = directive.level;
        }
    }
    return max_level;
}

bool Filter::matches(const Record &record) const {
    if (!this->enabled(record.get_metadata())) {
        return false;
    }
    if (filter_) {
        return filter_->is_match(record.get_args().str());
    }
    return true;
}

bool Filter::enabled(const Metadata &metadata) const {
  return ::enabled(directives, metadata.get_level(), metadata.get_target());
}

// FilterBuilder methods
FilterBuilder &FilterBuilder::add_directive(const Directive &directive) {
    for (auto &d : directives) {
        if (d.name == directive.name) {
            d.level = directive.level;
            return *this;
        }
    }
    directives.push_back(directive);
    return *this;
}

FilterBuilder &FilterBuilder::set_filter(const FilterOp &filter) {
    filter_ = filter;
    return *this;
}

Filter FilterBuilder::build() {
    if (directives.empty()) {
        directives.push_back(Directive(boost::none, Level(Level::LevelType::TRACE)));
    }
    std::sort(directives.begin(), directives.end(), [](const Directive &a, const Directive &b) {
        return a.name < b.name;
    });
    return Filter(directives, filter_);
}

