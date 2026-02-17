#include "csvtool/aggregation_engine.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(AggregationEngineTest, SingleGroupSum) {
  // Group by col 0 (dept), Sum col 1 (salary)
  csvtool::AggregationCondition condition;
  condition.group_by_index = 0;
  condition.agg_column_index = 1;
  condition.agg_type = "sum";

  csvtool::AggregationEngine engine(condition);

  // 3 rows, same group
  engine.aggregate({"Eng", "1000"});
  engine.aggregate({"Eng", "2000"});
  engine.aggregate({"Eng", "3000"});

  auto result = engine.get_result();
  ASSERT_EQ(result.size(), 1);
  EXPECT_DOUBLE_EQ(result["Eng"].sum, 6000.0);
}

TEST(AggregationEngineTest, MultipleGroupsCount) {
  // Group by col 0 (dept), Count col 1 (salary - unused for count but needed
  // index)
  csvtool::AggregationCondition condition;
  condition.group_by_index = 0;
  condition.agg_column_index = 1;
  condition.agg_type = "count";

  csvtool::AggregationEngine engine(condition);

  engine.aggregate({"Eng", "100"});
  engine.aggregate({"HR", "100"});
  engine.aggregate({"Eng", "100"});
  engine.aggregate({"Sales", "100"});
  engine.aggregate({"HR", "100"});

  auto result = engine.get_result();
  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result["Eng"].count, 2);
  EXPECT_EQ(result["HR"].count, 2);
  EXPECT_EQ(result["Sales"].count, 1);
}

TEST(AggregationEngineTest, MinMaxTracking) {
  // Group by col 0, Min/Max col 1
  csvtool::AggregationCondition condition;
  condition.group_by_index = 0;
  condition.agg_column_index = 1;
  // Type doesn't matter for internal accumulation, it tracks everything
  condition.agg_type = "min";

  csvtool::AggregationEngine engine(condition);

  engine.aggregate({"A", "10"});
  engine.aggregate({"A", "50"});
  engine.aggregate({"A", "5"});
  engine.aggregate({"A", "20"});

  auto result = engine.get_result();
  EXPECT_DOUBLE_EQ(result["A"].min, 5.0);
  EXPECT_DOUBLE_EQ(result["A"].max, 50.0);
}

TEST(AggregationEngineTest, AverageCalculation) {
  // Group by col 0, Avg col 1
  // Engine computes sum and count, we derive avg from that in checking or
  // output
  csvtool::AggregationCondition condition;
  condition.group_by_index = 0;
  condition.agg_column_index = 1;
  condition.agg_type = "avg";

  csvtool::AggregationEngine engine(condition);

  engine.aggregate({"A", "10"});
  engine.aggregate({"A", "20"});

  auto result = engine.get_result();
  double avg = result["A"].sum / result["A"].count;
  EXPECT_DOUBLE_EQ(avg, 15.0);
}
