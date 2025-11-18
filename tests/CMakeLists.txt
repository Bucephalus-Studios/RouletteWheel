cmake_minimum_required(VERSION 3.14)
project(RouletteWheelTests VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create interface library for header-only RouletteWheel
add_library(roulette_wheel INTERFACE)
target_include_directories(roulette_wheel INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Download and configure Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# Test executable
add_executable(roulette_wheel_tests
    test_wheel_region.cpp
    test_roulette_wheel.cpp
    test_integration.cpp
)

target_link_libraries(roulette_wheel_tests
    PRIVATE
    roulette_wheel
    GTest::gtest
    GTest::gtest_main
)

target_compile_options(roulette_wheel_tests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(roulette_wheel_tests)
