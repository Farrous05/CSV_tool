cmake_minimum_required(VERSION 3.16)

project(csvtool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.4.2
)
FetchContent_MakeAvailable(cli11)

add_executable(csvtool src/main.cpp src/cli_parser.cpp )

target_link_libraries(csvtool PRIVATE CLI11::CLI11)

# Add include directories
target_include_directories(csvtool PUBLIC include)

