#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <boost/optional.hpp>
#include "Directive.h"
#include "FilterOp.h"
#include "../Record.h"
#include "../Level.h"

class Filter {
public:
    Filter() = default;
    Filter(const std::vector<Directive> &directives, const boost::optional<FilterOp> &filter);
    Level filter() const;
    bool matches(const Record &record) const;
    virtual ~Filter() = default;
    bool enabled(const Metadata &metadata) const;

private:
    std::vector<Directive> directives;
    boost::optional<FilterOp> filter_;
};

class FilterBuilder {
public:
    FilterBuilder &add_directive(const Directive &directive);
    FilterBuilder &set_filter(const FilterOp &filter);
    Filter build();

private:
    std::vector<Directive> directives;
    boost::optional<FilterOp> filter_;
};

#endif // FILTER_H
