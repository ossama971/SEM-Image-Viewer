#ifndef FILTER_OP_H
#define FILTER_OP_H

#include <string>
#include <boost/regex.hpp>

class FilterOp {
public:
    explicit FilterOp(const std::string &pattern);
    bool is_match(const std::string &target) const;
    virtual ~FilterOp() = default;

private:
    boost::regex pat;
};

#endif // FILTER_OP_H

