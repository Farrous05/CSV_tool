#ifndef AGGREGATION_CONDITION_H
#define AGGREGATION_CONDITION_H

#include <string>
#include <vector>

namespace csvtool {

struct AggregationCondition {
  size_t group_by_index;
  size_t agg_column_index;
  std::string agg_column;
  std::string agg_type;
  bool global_aggregation = false;
};

} // namespace csvtool

#endif