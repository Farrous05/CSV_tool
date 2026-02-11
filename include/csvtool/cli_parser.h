#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <string>


namespace csvtool {

struct Config {
    std::string input_file;
    char delimiter = ',';
    std::string filter_expression;
    std::string group_by_column;
    std::string aggregation_expression;
    int top_k = 0;
    std::string parse_error_strategy = "fail";
    int max_errors = 0;
};

Config parse(int argc, char* argv[]);

} // namespace csvtool

#endif