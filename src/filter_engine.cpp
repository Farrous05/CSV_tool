#include "csvtool/filter_engine.h"
#include <string>
#include <vector>

namespace csvtool {

FilterEngine::FilterEngine(FilterCondition condition) : condition(condition) {}

bool FilterEngine::evaluate(const std::vector<std::string> &row) {
  if (condition.column_index >= row.size()) {
    return false;
  }

  const std::string &cell_value = row[condition.column_index];
  const std::string &value = condition.value;
  const std::string &op = condition.op;

  // Try numeric comparison first if possible
  // TODO: avoid exception in hot path (build is_numeric(const std::string&)
  // helper)

  try {
    double cell_num = std::stod(cell_value);
    double val_num = std::stod(value);

    if (op == ">")
      return cell_num > val_num;
    if (op == "<")
      return cell_num < val_num;
    if (op == ">=")
      return cell_num >= val_num;
    if (op == "<=")
      return cell_num <= val_num;
    if (op == "=" || op == "==")
      return cell_num == val_num;
    if (op == "!=")
      return cell_num != val_num;
  } catch (...) {
    // Fallback to string comparison
  }

  if (op == ">")
    return cell_value > value;
  if (op == "<")
    return cell_value < value;
  if (op == ">=")
    return cell_value >= value;
  if (op == "<=")
    return cell_value <= value;
  if (op == "=" || op == "==")
    return cell_value == value;
  if (op == "!=")
    return cell_value != value;

  return false;
}

FilterCondition
parse_filter(const std::string &expression,
             std::function<size_t(const std::string &)> lookup_column) {
  std::vector<std::string> ops = {">=", "<=", "!=", "==", ">", "<", "="};
  std::string op;
  size_t op_pos = std::string::npos;

  for (const auto &o : ops) {
    op_pos = expression.find(o);
    if (op_pos != std::string::npos) {
      op = o;
      break;
    }
  }

  if (op_pos == std::string::npos) {
    throw std::runtime_error("Invalid filter expression: no operator found");
  }

  std::string col_name = expression.substr(0, op_pos);
  std::string value = expression.substr(op_pos + op.length());

  // Trim whitespace
  auto trim = [](std::string &s) {
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    s.erase(s.find_last_not_of(" \t\n\r") + 1);
  };
  trim(col_name);
  trim(value);

  // Find column index using callback
  size_t col_idx = lookup_column(col_name);

  return {col_idx, op, value};
}

} // namespace csvtool
