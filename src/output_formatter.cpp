#include "csvtool/output_formatter.h"
#include <iomanip>
#include <iostream>

namespace csvtool {

void Output_formatter(
    const std::string &group_by_column, const std::string &agg_column,
    const std::string &agg_type,
    const std::unordered_map<std::string, AggregationEngine::Accumulator>
        &results,
    const int top_k) {
  if (group_by_column.empty() && agg_column.empty()) {
    return;
  }
  std::vector<std::pair<std::string, AggregationEngine::Accumulator>>
      sorted_results(results.begin(), results.end());
  std::sort(sorted_results.begin(), sorted_results.end(),
            [&](const auto &a, const auto &b) {
              if (agg_type == "count") {
                return a.second.count > b.second.count;
              } else if (agg_type == "sum") {
                return a.second.sum > b.second.sum;
              } else if (agg_type == "min") {
                return a.second.min < b.second.min; // Ascending for min
              } else if (agg_type == "max") {
                return a.second.max > b.second.max;
              } else if (agg_type == "avg") {
                double avg_a =
                    a.second.count > 0 ? a.second.sum / a.second.count : 0.0;
                double avg_b =
                    b.second.count > 0 ? b.second.sum / b.second.count : 0.0;
                return avg_a > avg_b;
              }
              return a.second.sum > b.second.sum; // Default
            });

  // Print CSV header
  if (!group_by_column.empty()) {
    std::cout << group_by_column << ",";
  }
  std::cout << agg_type << "(" << agg_column << ")" << std::endl;

  int count = 0;
  for (const auto &[key, acc] : sorted_results) {
    if (top_k > 0 && count >= top_k) {
      break;
    }
    count++;

    // Print key if grouping
    if (!group_by_column.empty()) {
      std::cout << key << ",";
    }

    std::cout << std::fixed << std::setprecision(0);

    if (agg_type == "count") {
      std::cout << acc.count << std::endl;
    } else if (agg_type == "sum") {
      std::cout << std::fixed << std::setprecision(2) << acc.sum << std::endl;
    } else if (agg_type == "min") {
      std::cout << acc.min << std::endl;
    } else if (agg_type == "max") {
      std::cout << acc.max << std::endl;
    } else if (agg_type == "avg") {
      double avg = acc.count > 0 ? acc.sum / acc.count : 0.0;
      std::cout << std::fixed << std::setprecision(4) << avg << std::endl;
    } else {
      std::cout << acc.sum << std::endl;
    }
  }
}

} // namespace csvtool