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
