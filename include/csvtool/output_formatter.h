#ifndef OUTPUT_FORMATTER_H
#define OUTPUT_FORMATTER_H

#include "aggregation_engine.h"
#include <string>
#include <unordered_map>

namespace csvtool {

void Output_formatter(
    const std::string &group_by_column, const std::string &agg_column,
    const std::string &agg_type,
    const std::unordered_map<std::string, AggregationEngine::Accumulator>
        &results,
    const int top_k = 0);

} // namespace csvtool

#endif