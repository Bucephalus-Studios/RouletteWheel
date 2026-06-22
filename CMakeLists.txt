cmake_minimum_required(VERSION 3.16)
project(RouletteWheel LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(stevensMathLib
    GIT_REPOSITORY https://github.com/Bucephalus-Studios/stevensMathLib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(stevensMathLib)

add_library(RouletteWheel INTERFACE)
add_library(RouletteWheel::RouletteWheel ALIAS RouletteWheel)

target_include_directories(RouletteWheel INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(RouletteWheel INTERFACE cxx_std_17)
target_link_libraries(RouletteWheel INTERFACE stevensMathLib)
