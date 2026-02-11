#include <iostream>
#include "csvtool/cli_parser.h"

int main(int argc, char* argv[]) {
    csvtool::Config config = csvtool::parse(argc, argv);
    std::cout << "csvtool v0.1" << std::endl;
    if (!config.input_file.empty()) {
        std::cout << "Input file: " << config.input_file << std::endl;
        std::cout << "Delimiter: " << config.delimiter << std::endl;
        std::cout << "Filter expression: " << config.filter_expression << std::endl;
        std::cout << "Group by column: " << config.group_by_column << std::endl;
        std::cout << "Aggregation expression: " << config.aggregation_expression << std::endl;
        std::cout << "Top K: " << config.top_k << std::endl;
        std::cout << "Parse error strategy: " << config.parse_error_strategy << std::endl;
        std::cout << "Max errors: " << config.max_errors << std::endl;
    }

    return 0;
}