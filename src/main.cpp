#include "csvtool/aggregation_engine.h"
#include "csvtool/cli_parser.h"
#include "csvtool/filter_engine.h"
#include "csvtool/input_reader.h"
#include "csvtool/output_formatter.h"
#include "csvtool/row_parser.h"
#include <iostream>
#include <optional>
#include <stdexcept>

int main(int argc, char *argv[]) {
  csvtool::Config config = csvtool::parse(argc, argv);
  if (config.input_file.empty()) {
    std::cerr << "Usage: csvtool [input_file] [OPTIONS]\nTry './csvtool "
                 "--help' for more information."
              << std::endl;
    return 1;
  }
  // std::cout << "csvtool v0.1" << std::endl; // Silent by default
  std::optional<csvtool::AggregationEngine> agg_engine;
  std::optional<csvtool::FilterEngine> filter_engine;

  try {
    csvtool::InputReader reader(config.input_file);
    std::optional<std::string> line;
    // Read header to initialize parser
    auto header_line = reader.next_line();
    if (!header_line) {
      std::cerr << "Empty input file" << std::endl;
      return 1;
    }
    csvtool::RowParser parser(config.delimiter, header_line.value());

    if (!config.aggregation_expression.empty()) {
      auto lookup = [&](const std::string &col) {
        return parser.get_column_index(col);
      };

      try {
        csvtool::AggregationCondition condition =
            csvtool::parse_aggregation(config.aggregation_expression, lookup);

        if (!config.group_by_column.empty()) {
          condition.group_by_index =
              parser.get_column_index(config.group_by_column);
        } else {
          condition.global_aggregation = true;
        }

        agg_engine.emplace(condition);
      } catch (const std::exception &e) {
        std::cerr << "Error parsing aggregation: " << e.what() << std::endl;
        return 1;
      }
    }

    if (!config.filter_expression.empty()) {
      auto lookup = [&](const std::string &col) {
        return parser.get_column_index(col);
      };
      csvtool::FilterCondition condition =
          csvtool::parse_filter(config.filter_expression, lookup);
      filter_engine.emplace(condition);
    }

    // Print header if we are not aggregating
    if (!agg_engine) {
      std::cout << header_line.value() << std::endl;
    }

    int error_count = 0;
    std::vector<std::string> fields;
    while ((line = reader.next_line())) {
      parser.parse_row(line.value(), fields);

      bool row_valid = true;
      if (agg_engine) {
        if (!agg_engine->aggregate(fields)) {
          row_valid = false;
        }
      } else {
      }

      if (!row_valid) {
        if (config.parse_error_strategy == "fail") {
          std::cerr << "Row parsing/processing failed." << std::endl;
          return 1;
        } else if (config.parse_error_strategy == "warn") {
          std::cerr << "Warning: Row parsing/processing failed." << std::endl;
          error_count++;
        } else {
          // skip
          error_count++; // Still count for max_errors
        }

        if (config.max_errors > 0 && error_count > config.max_errors) {
          std::cerr << "Max errors exceeded." << std::endl;
          return 1;
        }
        continue;
      }

      // Filter check
      if (filter_engine && !filter_engine->evaluate(fields)) {
        continue;
      }

      if (!agg_engine) {
        // Normal row printing if no aggregation
        // We print the raw line to preserve CSV format
        std::cout << line.value() << std::endl;
      }
    }

    // Print aggregation results
    if (agg_engine) {
      auto results = agg_engine->get_result();
      const auto &condition = agg_engine->get_condition();
      csvtool::Output_formatter(config.group_by_column, condition.agg_column,
                                condition.agg_type, results, config.top_k);
    }

  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  if (config.verbose) {
    std::cout << "Input file: " << config.input_file << std::endl;
    std::cout << "Delimiter: " << config.delimiter << std::endl;
    std::cout << "Filter expression: " << config.filter_expression << std::endl;
    std::cout << "Group by column: " << config.group_by_column << std::endl;
    std::cout << "Aggregation expression: " << config.aggregation_expression
              << std::endl;
    std::cout << "Top K: " << config.top_k << std::endl;
    std::cout << "Parse error strategy: " << config.parse_error_strategy
              << std::endl;
    std::cout << "Max errors: " << config.max_errors << std::endl;
  }

  return 0;
}