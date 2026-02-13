#ifndef FILTER_CONDITION_H
#define FILTER_CONDITION_H

#include <cstddef>
#include <string>

namespace csvtool {

struct FilterCondition {
  size_t column_index;
  std::string op;
  std::string value;
};

} // namespace csvtool

#endif