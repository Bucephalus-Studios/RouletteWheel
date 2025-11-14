#include "../RouletteWheel.hpp"
#include <benchmark/benchmark.h>
#include <string>
#include <random>

// Benchmark: Selection from small wheel (5 elements)
static void BM_SelectionSmallWheel(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 5; ++i) {
        wheel.addRegion(i, i + 1);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionSmallWheel);

// Benchmark: Selection from medium wheel (50 elements)
static void BM_SelectionMediumWheel(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 50; ++i) {
        wheel.addRegion(i, i + 1);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionMediumWheel);

// Benchmark: Selection from large wheel (500 elements)
static void BM_SelectionLargeWheel(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 500; ++i) {
        wheel.addRegion(i, i + 1);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionLargeWheel);

// Benchmark: Selection from very large wheel (5000 elements)
static void BM_SelectionVeryLargeWheel(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 5000; ++i) {
        wheel.addRegion(i, i + 1);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionVeryLargeWheel);

// Benchmark: Selection with integers
static void BM_SelectionIntegerWeights(benchmark::State& state) {
    const int numElements = state.range(0);
    RouletteWheel<int, int> wheel;

    for (int i = 0; i < numElements; ++i) {
        wheel.addRegion(i, 100);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionIntegerWeights)->Range(10, 1000);

// Benchmark: Selection with floating-point weights
static void BM_SelectionFloatWeights(benchmark::State& state) {
    const int numElements = state.range(0);
    RouletteWheel<int, double> wheel;

    for (int i = 0; i < numElements; ++i) {
        wheel.addRegion(i, 100.0);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionFloatWeights)->Range(10, 1000);

// Benchmark: Selection with string elements
static void BM_SelectionStringElements(benchmark::State& state) {
    const int numElements = state.range(0);
    RouletteWheel<std::string, int> wheel;

    for (int i = 0; i < numElements; ++i) {
        wheel.addRegion("Element_" + std::to_string(i), 100);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionStringElements)->Range(10, 1000);

// Benchmark: Selection with highly skewed weights
static void BM_SelectionSkewedWeights(benchmark::State& state) {
    RouletteWheel<int, int> wheel;

    // One element with very high weight
    wheel.addRegion(0, 1000000);

    // Many elements with low weight
    for (int i = 1; i < 100; ++i) {
        wheel.addRegion(i, 1);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionSkewedWeights);

// Benchmark: Selection with equal weights
static void BM_SelectionEqualWeights(benchmark::State& state) {
    const int numElements = state.range(0);
    RouletteWheel<int, int> wheel;

    for (int i = 0; i < numElements; ++i) {
        wheel.addRegion(i, 1);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionEqualWeights)->Range(10, 1000);

// Benchmark: SelectAndRemove operation
static void BM_SelectAndRemove(benchmark::State& state) {
    const int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        for (int i = 0; i < numElements; ++i) {
            wheel.addRegion(i, 100);
        }
        state.ResumeTiming();

        while (!wheel.empty()) {
            benchmark::DoNotOptimize(wheel.selectAndRemove());
        }
    }

    state.SetItemsProcessed(state.iterations() * numElements);
}
BENCHMARK(BM_SelectAndRemove)->Range(10, 500);

// Benchmark: SelectAndModifyWeight operation
static void BM_SelectAndModifyWeight(benchmark::State& state) {
    const int numElements = 100;

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        for (int i = 0; i < numElements; ++i) {
            wheel.addRegion(i, 100);
        }
        state.ResumeTiming();

        for (int i = 0; i < 50; ++i) {
            benchmark::DoNotOptimize(wheel.selectAndModifyWeight(-1));
        }
    }
}
BENCHMARK(BM_SelectAndModifyWeight);

// Benchmark: SelectSafe (with optional return)
static void BM_SelectSafe(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 100; ++i) {
        wheel.addRegion(i, 100);
    }

    for (auto _ : state) {
        auto result = wheel.selectSafe();
        benchmark::DoNotOptimize(result);
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectSafe);

// Benchmark: Worst-case selection (always selects last element)
static void BM_SelectionWorstCase(benchmark::State& state) {
    const int numElements = state.range(0);
    RouletteWheel<int, double> wheel;

    // Add elements with increasing weights, last one has highest
    for (int i = 0; i < numElements - 1; ++i) {
        wheel.addRegion(i, 0.00001);  // Very small weight
    }
    wheel.addRegion(numElements - 1, 1000000.0);  // Very large weight

    // Seed to always hit the last element
    wheel.seedRandom(42);

    for (auto _ : state) {
        benchmark::DoNotOptimize(wheel.select());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_SelectionWorstCase)->Range(10, 1000);
