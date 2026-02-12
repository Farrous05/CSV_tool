#include "csvtool/input_reader.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace csvtool {

InputReader::InputReader(const std::string &input_file) {
  file.open(input_file);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + input_file);
  }
}

std::optional<std::string> InputReader::next_line() {

  std::string line;
  if (std::getline(file, line)) {
    return line;
  }
  return std::nullopt;
}

} // namespace csvtool
