#include "csvtool/cli_parser.h"
#include <CLI/CLI.hpp>
#include <cstdlib>

namespace csvtool {

Config parse(int argc, char* argv[]) {

    Config config;

    CLI::App app("csvtool");
    app.set_help_flag("-h,--help");
    app.add_option("input_file", config.input_file, "Input file");
    app.add_option("--delimiter", config.delimiter, "Delimiter");
    app.add_option("--filter", config.filter_expression, "Filter expression");
    app.add_option("--group-by", config.group_by_column, "Group by column");
    app.add_option("--agg", config.aggregation_expression, "Aggregation expression");
    app.add_option("--top", config.top_k, "Top K");
    app.add_option("--on-parse-error", config.parse_error_strategy, "Parse error strategy");
    app.add_option("--max-errors", config.max_errors, "Max errors");
    
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        std::exit(app.exit(e));
    }

    return config;
}

} // namespace csvtool