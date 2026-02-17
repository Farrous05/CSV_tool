#include "csvtool/row_parser.h"
#include <algorithm> // for transform
#include <cctype>    // for tolower
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace csvtool {

// Helper to convert string to lowercase
static std::string to_lower(const std::string &s) {
  std::string lower_s = s;
  std::transform(lower_s.begin(), lower_s.end(), lower_s.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return lower_s;
}

RowParser::RowParser(char delimiter, const std::string &header_line) {
  this->delimiter = delimiter;
  std::vector<std::string> header;
  parse_row(header_line, header);
  for (size_t i = 0; i < header.size(); i++) {
    // Store lowercase version of header for case-insensitive lookup
    header_map[to_lower(header[i])] = i;
  }
}

void RowParser::parse_row(const std::string &line,
                          std::vector<std::string> &fields) {
  fields.clear();
  // handle ""
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
}

size_t RowParser::get_column_index(const std::string &column_name) {
  // Lookup using lowercase version of the column name
  std::string lower_name = to_lower(column_name);
  auto it = header_map.find(lower_name);
  if (it == header_map.end()) {
    throw std::runtime_error("Column not found: " + column_name);
  }
  return it->second;
}

} // namespace csvtool