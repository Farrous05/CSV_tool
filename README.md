# CSV Tool

A high-performance command-line utility for analyzing CSV files. It supports filtering, grouping, aggregation, and top-K queries in a single pass.

## Features

-   **Filtering**: Select rows based on numeric or string conditions (e.g., `salary > 50000`, `city == "Paris"`).
-   **Grouping**: Group rows by a specific column.
-   **Aggregation**: Compute statistics: `sum`, `avg`, `min`, `max`, `count` (e.g., `sum(salary)`).
-   **Global Aggregation**: Compute statistics over the whole file (no grouping needed).
-   **Top-K**: Retrieve the top K groups based on aggregated values.
-   **Efficient**: Designed for single-pass streaming processing.

## Build

Requirements:
-   CMake 3.16+
-   C++17 compatible compiler

```bash
mkdir build
cd build
cmake ..
make
```

To run tests:
```bash
./csvtool_tests
```

## Usage

```bash
./csvtool [input_file] [OPTIONS]
```

### Options

| Option | Description |
| :--- | :--- |
| `-h, --help` | Show help message |
| `--delimiter` | CSV delimiter (default: `,`) |
| `--filter` | Filter expression (e.g., `age > 25`) |
| `--group-by` | Column to group by (optional) |
| `--agg` | Aggregation expression (e.g., `avg(salary)`) |
| `--top` | Number of top results to return |
| `--verbose` | Print verbose debug information |

### Examples

**1. Print file content (cat-like):**
```bash
./csvtool employees.csv
```

**2. Filter rows:**
```bash
./csvtool employees.csv --filter "salary > 50000"
```

**3. Global aggregation (Sum of all salaries):**
```bash
./csvtool employees.csv --agg "sum(salary)"
```

**4. Group by department and calculate average salary:**
```bash
./csvtool employees.csv --group-by department --agg "avg(salary)"
```

**5. Complex query (Top 2 departments by total salary with filtering):**
```bash
./csvtool employees.csv --filter "age > 25" --group-by department --agg "sum(salary)" --top 2
```

## Supported Operations

-   **Comparisons**: `>`, `<`, `>=`, `<=`, `=`, `==`, `!=`
-   **Aggregations**: `sum`, `count`, `min`, `max`, `avg`
