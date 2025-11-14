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
