#ifndef FILTER_CONDITION_H
#define FILTER_CONDITION_H

#include <string>

namespace csvtool {

struct FilterCondition {
public:
  FilterCondition(const std::string &filter_expression);

private:
  std::string op;
  int column_index;
  std::string value;
};

} // namespace csvtool

#endif