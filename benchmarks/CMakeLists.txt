add_executable(benchmarks
    benchmark_operations.cpp
    benchmark_selection.cpp
    benchmark_construction.cpp
)

target_link_libraries(benchmarks
    PRIVATE
        RouletteWheel
        benchmark::benchmark_main
)