cmake_minimum_required(VERSION 3.16)

project(csvtool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch CLI11
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.4.2
)
FetchContent_MakeAvailable(cli11)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create the library
add_library(csvtool_lib 
    src/cli_parser.cpp 
    src/input_reader.cpp 
    src/row_parser.cpp 
    src/filter_engine.cpp 
    src/aggregation_engine.cpp 
    src/output_formatter.cpp
)

# Set include directories for the library
target_include_directories(csvtool_lib PUBLIC include)

# Link CLI11 to the library
target_link_libraries(csvtool_lib PUBLIC CLI11::CLI11)

# Main executable
add_executable(csvtool src/main.cpp)
target_link_libraries(csvtool PRIVATE csvtool_lib)

# Test executable
enable_testing()

add_executable(csvtool_tests 
    tests/test_row_parser.cpp
    tests/test_filter_engine.cpp
    tests/test_aggregation_engine.cpp
)
target_link_libraries(csvtool_tests PRIVATE csvtool_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(csvtool_tests)
