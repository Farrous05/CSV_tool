#ifndef INPUT_READER_H
#define INPUT_READER_H
#include <fstream>
#include <optional>
#include <string>

namespace csvtool {
class InputReader {
public:
  InputReader(const std::string &input_file);

  std::optional<std::string> next_line();

private:
  std::ifstream file;
};
} // namespace csvtool

#endif
