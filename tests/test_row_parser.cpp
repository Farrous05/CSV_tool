#include "csvtool/row_parser.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(RowParserTest, QuotedFields) {
  // "hello, world",30,Paris
  // Should be 3 fields
  std::string header = "msg,age,city";
  csvtool::RowParser parser(',', header);

  std::string line = "\"hello, world\",30,Paris";
  std::vector<std::string> row;
  parser.parse_row(line, row);

  ASSERT_EQ(row.size(), 3);
  EXPECT_EQ(row[0], "hello, world");
  EXPECT_EQ(row[1], "30");
  EXPECT_EQ(row[2], "Paris");
}

TEST(RowParserTest, EmptyFields) {
  // Alice,,Paris
  // Middle field empty
  std::string header = "name,age,city";
  csvtool::RowParser parser(',', header);

  std::string line = "Alice,,Paris";
  std::vector<std::string> row;
  parser.parse_row(line, row);

  ASSERT_EQ(row.size(), 3);
  EXPECT_EQ(row[0], "Alice");
  EXPECT_EQ(row[1], "");
  EXPECT_EQ(row[2], "Paris");
}

TEST(RowParserTest, ColumnIndexLookup) {
  // header: name,age,city
  // get_column_index("age") -> 1
  std::string header = "name,age,city";
  csvtool::RowParser parser(',', header);

  EXPECT_EQ(parser.get_column_index("name"), 0);
  EXPECT_EQ(parser.get_column_index("age"), 1);
  EXPECT_EQ(parser.get_column_index("city"), 2);
}

TEST(RowParserTest, MissingColumn) {
  // get_column_index("nonexistent") -> throws
  std::string header = "name,age,city";
  csvtool::RowParser parser(',', header);

  EXPECT_THROW(parser.get_column_index("nonexistent"), std::runtime_error);
}
