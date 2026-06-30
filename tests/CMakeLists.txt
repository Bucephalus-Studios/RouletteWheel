add_executable(tests
    test_wheel_region.cpp
    test_roulette_wheel.cpp
    test_integration.cpp
)

target_link_libraries(tests
    PRIVATE
        RouletteWheel
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)