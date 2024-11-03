#include "FilterOp.h"

FilterOp::FilterOp(const std::string &pattern) : pat(pattern) {}

bool FilterOp::is_match(const std::string &target) const {
    return boost::regex_search(target, pat);
}

