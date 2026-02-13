#include "csvtool/cli_parser.h"
#include "csvtool/filter_engine.h"
#include "csvtool/input_reader.h"
#include "csvtool/row_parser.h"
#include <iostream>
#include <optional>
#include <stdexcept>

int main(int argc, char *argv[]) {
  csvtool::Config config = csvtool::parse(argc, argv);
  std::cout << "csvtool v0.1" << std::endl;
  try {
    csvtool::InputReader reader(config.input_file);
    std::optional<std::string> line;
    csvtool::RowParser parser(config.delimiter, reader.next_line().value());

    std::optional<csvtool::FilterEngine> filter_engine;
    if (!config.filter_expression.empty()) {
      auto lookup = [&](const std::string &col) {
        return parser.get_column_index(col);
      };
      csvtool::FilterCondition condition =
          csvtool::parse_filter(config.filter_expression, lookup);
      filter_engine.emplace(condition);
    }

    while ((line = reader.next_line())) {
      std::vector<std::string> fields = parser.parse_row(line.value());
      if (filter_engine && !filter_engine->evaluate(fields)) {
        continue;
      }
      for (const auto &field : fields) {
        std::cout << field << " ";
      }
      std::cout << std::endl;
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