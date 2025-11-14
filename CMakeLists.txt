cmake_minimum_required(VERSION 3.14)
project(RouletteWheel VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create interface library for header-only library
add_library(roulette_wheel INTERFACE)
target_include_directories(roulette_wheel INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Option to build tests and benchmarks
option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(USE_CEREAL "Enable Cereal serialization support" OFF)

# Add cereal support if enabled
if(USE_CEREAL)
    target_compile_definitions(roulette_wheel INTERFACE USE_CEREAL)
endif()

# Download and configure Google Test
if(BUILD_TESTS)
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
    add_subdirectory(tests)
endif()

# Download and configure Google Benchmark
if(BUILD_BENCHMARKS)
    include(FetchContent)

    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )

    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googlebenchmark)

    add_subdirectory(benchmarks)
endif()

# Build examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation rules
install(TARGETS roulette_wheel
    EXPORT RouletteWheelTargets
)

install(FILES
    RouletteWheel.hpp
    DESTINATION include
)

install(DIRECTORY classes/
    DESTINATION include/classes
    FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT RouletteWheelTargets
    FILE RouletteWheelTargets.cmake
    NAMESPACE RouletteWheel::
    DESTINATION lib/cmake/RouletteWheel
)
