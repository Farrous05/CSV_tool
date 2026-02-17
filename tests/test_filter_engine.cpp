#include "csvtool/filter_engine.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Test fixture or helper if needed, but simple tests are fine

TEST(FilterEngineTest, NumericComparison) {
  // salary > 50000
  csvtool::FilterCondition condition{2, ">", "50000"};
  csvtool::FilterEngine engine(condition);

  // Row: name, department, salary
  std::vector<std::string> rowHigh = {"Alice", "Eng", "60000"};
  std::vector<std::string> rowLow = {"Bob", "HR", "40000"};

  EXPECT_TRUE(engine.evaluate(rowHigh));
  EXPECT_FALSE(engine.evaluate(rowLow));
}

TEST(FilterEngineTest, StringComparison) {
  // city == Paris
  csvtool::FilterCondition condition{1, "==", "Paris"};
  csvtool::FilterEngine engine(condition);

  // Row: name, city, age
  std::vector<std::string> rowParis = {"Alice", "Paris", "30"};
  std::vector<std::string> rowLondon = {"Bob", "London", "25"};

  EXPECT_TRUE(engine.evaluate(rowParis));
  EXPECT_FALSE(engine.evaluate(rowLondon));
}
