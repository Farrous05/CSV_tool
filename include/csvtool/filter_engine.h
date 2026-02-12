#ifndef FILTER_ENGINE_H
#define FILTER_ENGINE_H

#include "filter_condition.h"
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

} // namespace csvtool

#endif