#include "csvtool/row_parser.h"
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace csvtool {

RowParser::RowParser(char delimiter, const std::string &header_line) {
  this->delimiter = delimiter;
  std::vector<std::string> header = parse_row(header_line);
  for (size_t i = 0; i < header.size(); i++) {
    header_map[header[i]] = i;
  }
}

std::vector<std::string> RowParser::parse_row(const std::string &line) {
  // handle ""
  std::vector<std::string> fields;
  std::string field;
  bool in_quotes = false;
  for (char c : line) {
    if (c == '"') {
      in_quotes = !in_quotes;
    } else if (c == delimiter && !in_quotes) {
      fields.push_back(field);
      field.clear();
    } else {
      field += c;
    }
  }
  fields.push_back(field);
  return fields;
}

size_t RowParser::get_column_index(const std::string &column_name) {
  auto it = header_map.find(column_name);
  if (it == header_map.end()) {
    throw std::runtime_error("Column not found: " + column_name);
  }
  return it->second;
}

} // namespace csvtool