#ifndef UTILS_H
#define UTILS_H

#include <charconv>
#include <cstdlib>
#include <string>

namespace csvtool {

inline bool try_parse_double(const std::string &s, double &out) {
  if (s.empty())
    return false;

  // Try std::from_chars first (C++17)
  // Note: some compilers might not support floating point from_chars yet
  // If this fails to compile on older clang/gcc, we can use strtod

  const char *first = s.data();
  const char *last = s.data() + s.size();

  // Skip leading whitespace which from_chars doesn't handle, but stod does
  while (first != last && std::isspace(*first)) {
    first++;
  }
  if (first == last)
    return false;

#if defined(__cpp_lib_to_chars) || defined(__cpp_lib_from_chars)
  // It seems feature macros might not be reliable for FP support specifically
  // We will try using it, if build fails we'll switch to strtod.
  // Actually, to be safe and portable immediately without trial/error loop:
  // strtod does not throw and is fast enough compared to exceptions.
  // But from_chars is faster.
  // Let's stick to the plan but be aware of whitespace issue (from_chars
  // doesn't skip it).

  // Using strtod for now as it handles whitespace and is standard C
  // To truly use from_chars we need to handle whitespace manually.

  char *end;
  out = std::strtod(s.c_str(), &end);
  return end != s.c_str();
#else
  // Fallback or preferred?
  // Let's implement using strtod for max portability and no exceptions
  char *end;
  out = std::strtod(s.c_str(), &end);
  // basic check: valid if we consumed something.
  // stricter check: we consumed everything? stod allows trailing garbage?
  // stod: "processes as many characters as possible"
  return end != s.c_str();
#endif
}

// Actually, let's write a version that tries to behave like we want:
// Return true if valid number found.

inline bool fast_parse_double(const std::string &s, double &out) {
  if (s.empty())
    return false;
  char *end;
  out = std::strtod(s.c_str(), &end);
  return end > s.c_str(); // Success if we parsed at least one character
}

} // namespace csvtool

#endif
