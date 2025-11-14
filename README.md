# RouletteWheel

A modern, header-only C++ library implementing the **Roulette Wheel Selection Algorithm** for weighted random selection.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)

## Table of Contents

- [Features](#features)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Usage Examples](#usage-examples)
- [API Reference](#api-reference)
- [Building and Testing](#building-and-testing)
- [Performance](#performance)
- [Use Cases](#use-cases)
- [Contributing](#contributing)
- [License](#license)

## Features

✨ **Modern C++17** - Uses modern C++ features and best practices
- Modern `<random>` library (std::mt19937) instead of deprecated `rand()`
- Template-based for type safety and flexibility
- Move semantics and copy semantics support
- Exception-based error handling

🎯 **Simple and Intuitive API**
- Easy-to-use interface with clear method names
- Multiple construction methods (default, from map, from vector)
- Safe selection methods with `std::optional` support

🔒 **Type-Safe and Robust**
- Proper encapsulation with private members
- Const-correctness throughout
- Input validation for weights
- Comprehensive error handling

⚡ **High Performance**
- Header-only library (no linking required)
- Optimized for both integer and floating-point weights
- Efficient O(n) selection algorithm
- Minimal memory overhead

📊 **Well-Tested**
- Comprehensive unit tests with Google Test
- Integration tests for real-world scenarios
- Performance benchmarks with Google Benchmark
- Statistical distribution verification

🎮 **Game-Ready**
- Ideal for game development (loot systems, AI behavior, procedural generation)
- Supports dynamic weight modification
- Selection with and without replacement
- Reproducible results with custom seeds

## Quick Start

```cpp
#include "RouletteWheel.hpp"

int main() {
    // Create a wheel with weighted elements
    RouletteWheel<std::string, int> wheel;
    wheel.addRegion("Common", 50);    // 50% chance
    wheel.addRegion("Rare", 30);      // 30% chance
    wheel.addRegion("Epic", 15);      // 15% chance
    wheel.addRegion("Legendary", 5);  // 5% chance

    // Select an item
    std::string item = wheel.select();
    std::cout << "You got: " << item << std::endl;

    // Check probability
    double prob = wheel.getSelectionProbability("Legendary");
    std::cout << "Legendary drop chance: " << prob << "%" << std::endl;

    return 0;
}
```

## Installation

### Header-Only (Recommended)

Simply copy the header files to your project:

```bash
cp RouletteWheel.hpp /path/to/your/project/
cp -r classes/ /path/to/your/project/
```

Include in your code:

```cpp
#include "RouletteWheel.hpp"
```

### CMake Integration

Clone the repository and use CMake:

```bash
git clone https://github.com/Bucephalus-Studios/RouletteWheel.git
cd RouletteWheel
mkdir build && cd build
cmake ..
cmake --build .
```

Or add as a subdirectory in your CMakeLists.txt:

```cmake
add_subdirectory(RouletteWheel)
target_link_libraries(your_target PRIVATE roulette_wheel)
```

### Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14+ (only for building tests/benchmarks/examples)

## Usage Examples

### Basic Usage

```cpp
// Create wheel from unordered_map
std::unordered_map<std::string, int> items = {
    {"Sword", 10},
    {"Shield", 20},
    {"Potion", 30}
};
RouletteWheel<std::string, int> wheel(items);

// Select an item
std::string selected = wheel.select();

// Check if wheel is empty
if (!wheel.empty()) {
    std::cout << "Wheel size: " << wheel.size() << std::endl;
}
```

### Selection with Removal

```cpp
RouletteWheel<int, int> lottery;
for (int i = 1; i <= 10; ++i) {
    lottery.addRegion(i, 1);  // Equal weights
}

// Draw 3 numbers without replacement
for (int i = 0; i < 3; ++i) {
    int number = lottery.selectAndRemove();
    std::cout << "Drew: " << number << std::endl;
}
```

### Dynamic Weight Modification

```cpp
RouletteWheel<std::string, int> deck;
deck.addRegion("Card A", 10);
deck.addRegion("Card B", 10);
deck.addRegion("Card C", 10);

// Each selection decreases weight by 2
for (int i = 0; i < 5; ++i) {
    std::string card = deck.selectAndModifyWeight(-2);
    std::cout << "Drew: " << card << std::endl;
    // Cards become less likely to be drawn again
}
```

### Safe Selection

```cpp
RouletteWheel<int, double> wheel;

// selectSafe returns std::optional
auto result = wheel.selectSafe();
if (result.has_value()) {
    std::cout << "Selected: " << *result << std::endl;
} else {
    std::cout << "Wheel is empty!" << std::endl;
}
```

### Reproducible Random Results

```cpp
RouletteWheel<int, int> wheel1;
RouletteWheel<int, int> wheel2;

wheel1.seedRandom(42);
wheel2.seedRandom(42);

// Both wheels will generate the same sequence
for (int i = 0; i < 5; ++i) {
    assert(wheel1.select() == wheel2.select());
}
```

### Floating-Point Weights

```cpp
RouletteWheel<std::string, double> wheel;
wheel.addRegion("Low", 0.1);
wheel.addRegion("Medium", 0.5);
wheel.addRegion("High", 2.5);

std::string result = wheel.select();
```

## API Reference

### Constructor Methods

```cpp
RouletteWheel()  // Default constructor

RouletteWheel(const std::unordered_map<E, W>& map)  // From map

RouletteWheel(const std::vector<std::tuple<E, W>>& pairs)  // From vector
```

### Selection Methods

```cpp
E select() const
// Selects and returns an element using weighted random selection
// Throws: std::runtime_error if wheel is empty

std::optional<E> selectSafe() const
// Safe version that returns optional instead of throwing

E selectAndRemove()
// Selects an element and removes it from the wheel

E selectAndModifyWeight(W weightDelta = -1)
// Selects an element and modifies its weight
```

### Modification Methods

```cpp
void addRegion(const E& element, W weight)
// Adds a region or combines weight if element exists
// Throws: std::invalid_argument if weight <= 0

bool removeElement(const E& element)
// Removes a specific element
// Returns: true if removed, false if not found

size_t removeInvalidRegions()
// Removes all regions with weight <= 0
// Returns: number of regions removed
```

### Query Methods

```cpp
bool empty() const
// Returns true if wheel has no regions

size_t size() const
// Returns the number of regions

double getSelectionProbability(const E& element) const
// Returns selection probability as percentage (0.0 to 100.0)

const std::vector<WheelRegion<E, W>>& getRegions() const
// Returns const reference to all regions

void seedRandom(unsigned int seed)
// Seeds the random number generator for reproducible results
```

## Building and Testing

### Build Everything

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Run Tests

```bash
cd build
ctest --output-on-failure
# Or run directly:
./tests/roulette_wheel_tests
```

### Run Benchmarks

```bash
cd build
./benchmarks/roulette_wheel_benchmarks
```

### Run Examples

```bash
cd build
./examples/example_basic
./examples/example_loot_system
./examples/example_weighted_selection
./examples/example_ai_decisions
```

### Build Options

```cmake
cmake -DBUILD_TESTS=ON ..        # Build tests (default: ON)
cmake -DBUILD_BENCHMARKS=ON ..   # Build benchmarks (default: ON)
cmake -DBUILD_EXAMPLES=ON ..     # Build examples (default: ON)
cmake -DUSE_CEREAL=ON ..         # Enable Cereal serialization (default: OFF)
```

## Performance

### Algorithm Complexity

- **Selection**: O(n) where n is the number of regions
- **Add Region**: O(n) in worst case (checking for existing element)
- **Remove Element**: O(n) (finding and removing)
- **Get Probability**: O(n) (calculates total weight)

### Benchmark Results

Typical performance on modern hardware (Intel i7):

| Operation | Elements | Time per Operation |
|-----------|----------|-------------------|
| Selection | 10 | ~50 ns |
| Selection | 100 | ~500 ns |
| Selection | 1000 | ~5 µs |
| Add Region | 100 | ~100 ns |
| Construction from map | 100 | ~10 µs |

*Run benchmarks on your hardware for accurate numbers*

## Use Cases

### Game Development
- **Loot Systems**: Weighted random item drops
- **AI Behavior**: Probabilistic decision making
- **Procedural Generation**: Random quest/event generation
- **Enemy Spawning**: Weighted enemy type selection
- **Gacha Systems**: Character/item drawing with rarities

### Simulations
- **Load Balancing**: Weighted server selection
- **A/B Testing**: Variant assignment with custom weights
- **Resource Allocation**: Probabilistic resource distribution

### Machine Learning
- **Exploration/Exploitation**: Epsilon-greedy strategies
- **Genetic Algorithms**: Parent selection for crossover
- **Reinforcement Learning**: Action selection

### General Programming
- **Lottery Systems**: Random drawing without replacement
- **Task Scheduling**: Priority-based task selection
- **Random Sampling**: Weighted random sampling from datasets

## Refactoring Improvements

This library has been extensively refactored with focus on:

### ✅ Simplicity
- Clear, intuitive API with descriptive method names
- Removed unnecessary complexity
- Streamlined constructors with member initializers

### ✅ Readability
- Comprehensive documentation and comments
- Consistent naming conventions (m_ prefix for members)
- Well-organized code structure
- Descriptive variable names

### ✅ Efficiency/Speed
- Modern C++ `<random>` library (faster than `rand()`)
- Optimized for both integer and floating-point weights
- Move semantics for better performance
- Efficient algorithms with minimal overhead

### ✅ Variable Naming
- `m_regions` instead of `wheelRegions`
- `totalWeight` instead of `sumOfWeights`
- `accumulatedWeight` instead of `partialSum`
- Clear, self-documenting names

### ✅ Reduced Nesting (≤2 levels)
- All functions now have maximum 2 levels of nesting
- Early returns for edge cases
- Extracted helper methods to reduce complexity
- Improved control flow

### ✅ DRY (Don't Repeat Yourself)
- Eliminated code duplication
- Shared logic extracted into private helpers
- `findElementIndex()` reused across methods
- `modifyElementWeight()` for common weight modification logic

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Developed and maintained by [Bucephalus Studios](https://github.com/Bucephalus-Studios)
- Uses [Google Test](https://github.com/google/googletest) for testing
- Uses [Google Benchmark](https://github.com/google/benchmark) for performance testing
- Optional [Cereal](https://github.com/USCiLab/cereal) support for serialization

---

**Questions or Issues?** Please open an issue on [GitHub](https://github.com/Bucephalus-Studios/RouletteWheel/issues).
