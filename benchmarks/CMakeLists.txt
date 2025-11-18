cmake_minimum_required(VERSION 3.14)
project(RouletteWheelBenchmarks VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create interface library for header-only RouletteWheel
add_library(roulette_wheel INTERFACE)
target_include_directories(roulette_wheel INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Download and configure Google Benchmark
include(FetchContent)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googlebenchmark)

# Benchmark executable
add_executable(roulette_wheel_benchmarks
    benchmark_selection.cpp
    benchmark_construction.cpp
    benchmark_operations.cpp
)

target_link_libraries(roulette_wheel_benchmarks
    PRIVATE
    roulette_wheel
    benchmark::benchmark
    benchmark::benchmark_main
)

target_compile_options(roulette_wheel_benchmarks PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /O2>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -O3>
)
