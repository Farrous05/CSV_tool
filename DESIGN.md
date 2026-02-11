# CSV Analyzer — Design Document

## 1. Input Contract
- Delimiter: comma by default, configurable via `--delimiter`
- Quoted fields: supported (e.g., `"hello, world"` is one field)
- Header row: always required (first line is column names)
- Encoding: UTF-8 assumed

## 2. CLI Interface (flag-based)

### Example commands
1. Filter only:
   `./csvtool --filter "salary > 50000" employees.csv`
2. Group-by + aggregation:
   `./csvtool --group-by department --agg avg(salary) employees.csv`
3. Filter + group-by + aggregation + top-K:
   `./csvtool --filter "age > 25" --group-by department --agg sum(salary) --top 2 employees.csv`

## 3. Operations (v1)
- Filter: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Group-by: single key
- Aggregations: count, sum, mean (avg), min, max
- Top-K: top K groups by aggregated value

## 4. Output
- Default: CSV-style with header row to stdout
- Example output for group-by + avg(salary):
```
  department,avg(salary)
  Engineering,63000
  Marketing,45000
```

## 5. Scale Target
- Target: up to 1GB input files
- Strategy: single-pass streaming
- Memory usage proportional to number of unique groups, not row count
- Only one row held in memory at a time during processing

## 6. Pipeline Architecture

main
 └─ CLIParser (stateless)
     └─ Config (owns parsed options: file path, filters, group-by keys, aggs, output format, strictness)

Pipeline (streaming):
InputReader (owns I/O buffer)
  → RowParser (stateless; produces rows from raw lines)
    → FilterEngine (stateless; applies compiled predicates from Config)
      → AggregationEngine (stateful; owns hash map of group → running accumulators)
        → OutputFormatter (stateless; writes CSV to stdout)

Ownership:
- InputReader owns the read buffer
- RowParser produces row data from the buffer
- FilterEngine borrows row data, does not copy
- AggregationEngine owns the aggregation state (hash maps, counters)
- OutputFormatter borrows final results, writes to ostream
- Row data uses string_view (non-owning) for zero-copy parsing
- Group-by keys are copied into owned storage in AggregationEngine when a row passes the filter

## 7. Error Philosophy
- Fatal errors (can't open file, bad CLI, OOM): exceptions
- Per-row errors (bad parse, type mismatch): `expected<T, E>` or status enum
- Strictness modes via `--on-parse-error=skip|warn|fail`
- `--max-errors=N` to cap error reporting
- Fast path stays exception-free
- Errors are counted and summarized, not thrown per row