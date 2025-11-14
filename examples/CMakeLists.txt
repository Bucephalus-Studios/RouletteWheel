# Example executables

# Basic usage example
add_executable(example_basic basic_usage.cpp)
target_link_libraries(example_basic PRIVATE roulette_wheel)

# Game loot system example
add_executable(example_loot_system loot_system.cpp)
target_link_libraries(example_loot_system PRIVATE roulette_wheel)

# Weighted random selection example
add_executable(example_weighted_selection weighted_selection.cpp)
target_link_libraries(example_weighted_selection PRIVATE roulette_wheel)

# AI decision making example
add_executable(example_ai_decisions ai_decisions.cpp)
target_link_libraries(example_ai_decisions PRIVATE roulette_wheel)
