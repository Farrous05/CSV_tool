#!/bin/bash
set -e

# Setup
BUILD_DIR=./build
CSVTOOL=$BUILD_DIR/csvtool
TEST_FILE=integration_test.csv
BAD_FILE=malformed.csv

# Compile
cmake --build $BUILD_DIR

echo "Generating test data..."
echo "name,age,salary" > $TEST_FILE
echo "Alice,30,50000" >> $TEST_FILE
echo "Bob,25,60000" >> $TEST_FILE
echo "Charlie,35,70000" >> $TEST_FILE

echo "Generating malformed data..."
echo "name,age,salary" > $BAD_FILE
echo "Alice,30,50000" >> $BAD_FILE
echo "Bob,Nan,60000" >> $BAD_FILE # Bad number
echo "Charlie,35" >> $BAD_FILE # Missing column

echo "--- Test 1: Happy Path (Filter) ---"
$CSVTOOL $TEST_FILE --filter "age > 28" > output.csv
count=$(wc -l < output.csv | tr -d ' ')
if [ "$count" != "3" ]; then # Header + 2 rows
    echo "Test 1 Failed: Expected 3 lines, got $count"
    cat output.csv
    exit 1
fi
echo "Pass"

echo "--- Test 2: Global Aggregation ---"
$CSVTOOL $TEST_FILE --agg "sum(salary)" > output.csv
grep -q "180000" output.csv || (echo "Test 2 Failed"; exit 1)
echo "Pass"

echo "--- Test 3: Standard Aggregation ---"
$CSVTOOL $TEST_FILE --group-by name --agg "max(age)" > output.csv
grep -q "Alice,30" output.csv || (echo "Test 3 Failed"; exit 1)
echo "Pass"

echo "--- Test 4: Strictness - Skip ---"
# Should skip bad rows and print result for good ones.
# Explicitly pass skip mode.
$CSVTOOL $BAD_FILE --agg "sum(salary)" --on-parse-error skip > output.csv
# Bob has bad age (not salary)? Wait, global agg sum(salary) strictly checks accessed columns.
# But RowParser parses strict columns? No, it just splits.
# AggregationEngine checks if agg_column_index exists.
# Row 3 "Charlie,35" has only 2 cols. Salary is index 2. So it fails (out of bounds).
# Row 2 "Bob,NaN,60000". Salary (60000) is fine. Age is NaN but we are not filtering on Age.
# So Row 2 should be included in sum(salary).
# Expected sum: 50000 + 60000 = 110000.
grep -q "110000" output.csv || (echo "Test 4 Failed: Expected 110000 (Bob included)"; cat output.csv; exit 1)
echo "Pass"

echo "--- Test 5: Strictness - Warn ---"
# Should print warning to stderr
$CSVTOOL $BAD_FILE --agg "sum(salary)" --on-parse-error warn > output.csv 2> stderr.txt
if ! grep -q "Warning" stderr.txt; then
    echo "Test 5 Failed: Expected warnings"
    cat stderr.txt
    exit 1
fi
echo "Pass"

echo "--- Test 6: Strictness - Fail ---"
set +e
$CSVTOOL $BAD_FILE --agg "sum(salary)" --on-parse-error fail > /dev/null 2>&1
status=$?
set -e
if [ "$status" -ne 1 ]; then
    echo "Test 6 Failed: Expected exit code 1"
    exit 1
fi
echo "Pass"

echo "Values verified. Cleanup."
rm $TEST_FILE $BAD_FILE output.csv stderr.txt
echo "All integration tests passed!"
