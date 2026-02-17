#include "csvtool/aggregation_engine.h"
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace csvtool {

AggregationEngine::AggregationEngine(AggregationCondition condition)
    : condition(condition) {}

void AggregationEngine::aggregate(const std::vector<std::string> &row) {
  if ((!condition.global_aggregation &&
       condition.group_by_index >= row.size()) ||
      condition.agg_column_index >= row.size()) {
    return;
  }

  std::string group_key;
  if (condition.global_aggregation) {
    group_key = ""; // Single global group
  } else {
    group_key = row[condition.group_by_index];
  }

  const std::string &agg_value_str = row[condition.agg_column_index];

  double agg_value;
  // TODO try/catch in hot path
  try {
    agg_value = std::stod(agg_value_str);
  } catch (...) {
    return; // Skip non-numeric values
  }

  auto &acc = accumulators[group_key];
  acc.sum += agg_value;
  acc.count++;
  if (agg_value < acc.min)
    acc.min = agg_value;
  if (agg_value > acc.max)
    acc.max = agg_value;
}

std::unordered_map<std::string, AggregationEngine::Accumulator>
AggregationEngine::get_result() const {
  return accumulators;
}

const AggregationCondition &AggregationEngine::get_condition() const {
  return condition;
}

AggregationCondition
parse_aggregation(const std::string &expression,
                  std::function<size_t(const std::string &)> lookup_column) {
  // Expected format: operation(column_name)
  size_t open_paren = expression.find('(');
  size_t close_paren = expression.find(')');

  if (open_paren == std::string::npos || close_paren == std::string::npos ||
      close_paren < open_paren) {
    throw std::runtime_error(
        "Invalid aggregation expression: expected format op(col)");
  }

  std::string op = expression.substr(0, open_paren);
  std::string col_name =
      expression.substr(open_paren + 1, close_paren - open_paren - 1);

  // Trim whitespace
  auto trim = [](std::string &s) {
    if (s.empty())
      return;
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    s.erase(s.find_last_not_of(" \t\n\r") + 1);
  };
  trim(op);
  trim(col_name);

  // Validate operator
  std::vector<std::string> valid_ops = {"sum", "count", "min", "max", "avg"};
  bool is_valid = false;
  for (const auto &valid_op : valid_ops) {
    if (op == valid_op) {
      is_valid = true;
      break;
    }
  }

  if (!is_valid) {
    throw std::runtime_error("Invalid aggregation operator: " + op);
  }

  // Find aggregation column index
  size_t agg_col_idx = lookup_column(col_name);

  AggregationCondition cond;
  cond.group_by_index = 0; // Placeholder, set by caller
  cond.agg_column_index = agg_col_idx;
  cond.agg_column = col_name;
  cond.agg_type = op;
  return cond;
}

} // namespace csvtool