#ifndef AGGREGATION_ENGINE_H
#define AGGREGATION_ENGINE_H

#include "aggregation_condition.h"
#include <functional>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

namespace csvtool {

class AggregationEngine {
public:
  struct Accumulator {
    double sum = 0;
    size_t count = 0;
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
  };

  AggregationEngine(AggregationCondition condition);
  void aggregate(const std::vector<std::string> &row);
  std::unordered_map<std::string, Accumulator> get_result() const;
  const AggregationCondition &get_condition() const;

private:
  AggregationCondition condition;
  std::unordered_map<std::string, Accumulator> accumulators;
};
AggregationCondition
parse_aggregation(const std::string &expression,
                  std::function<size_t(const std::string &)> lookup_column);
} // namespace csvtool

#endif