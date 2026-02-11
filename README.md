# CSV Tool

A command-line utility for analyzing CSV files, supporting filtering, grouping, aggregation, and top-K queries.

## Features

-   **Filtering**: Select rows based on conditions (e.g., `age > 25`).
-   **Grouping**: Group rows by a specific column.
-   **Aggregation**: Compute statistics like `sum`, `avg`, `min`, `max`, `count`.
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

## Usage

```bash
./build/csvtool [OPTIONS] [input_file]
```

### Options

| Option | Description |
| :--- | :--- |
| `-h, --help` | Show help message |
| `--delimiter` | CSV delimiter (default: `,`) |
| `--filter` | Filter expression (e.g., `salary > 50000`) |
| `--group-by` | Column to group by |
| `--agg` | Aggregation expression (e.g., `avg(salary)`) |
| `--top` | Number of top results to return |
| `--on-parse-error` | Strategy for handling parse errors (`fail`, `skip`, `warn`) |
| `--max-errors` | Maximum number of errors to report |

### Examples

**Filter only:**
```bash
./build/csvtool --filter "salary > 50000" employees.csv
```

**Group by department and calculate average salary:**
```bash
./build/csvtool --group-by department --agg avg(salary) employees.csv
```

**Complex query:**
```bash
./build/csvtool --filter "age > 25" --group-by department --agg sum(salary) --top 2 employees.csv
```

## Design

See [DESIGN.md](DESIGN.md) for architectural details and future plans.
