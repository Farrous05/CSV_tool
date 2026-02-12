#ifndef ROW_PARSER_H
#define ROW_PARSER_H

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace csvtool {

class RowParser {
public:
  RowParser(char delimiter, const std::string &header_line);

  std::vector<std::string> parse_row(const std::string &line);
  size_t get_column_index(const std::string &column_name);

private:
  char delimiter;
  std::unordered_map<std::string, size_t> header_map;
};

} // namespace csvtool

#endif