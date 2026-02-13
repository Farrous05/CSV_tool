#ifndef FILTER_ENGINE_H
#define FILTER_ENGINE_H

#include "filter_condition.h"
#include <functional>
#include <string>
#include <vector>

namespace csvtool {

class FilterEngine {
public:
  FilterEngine(FilterCondition condition);
  bool evaluate(const std::vector<std::string> &row);

private:
  FilterCondition condition;
};

FilterCondition
parse_filter(const std::string &expression,
             std::function<size_t(const std::string &)> lookup_column);

} // namespace csvtool

#endif