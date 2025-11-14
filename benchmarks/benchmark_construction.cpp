#include "../RouletteWheel.hpp"
#include <benchmark/benchmark.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>

// Benchmark: Default construction
static void BM_ConstructionDefault(benchmark::State& state) {
    for (auto _ : state) {
        RouletteWheel<int, int> wheel;
        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_ConstructionDefault);

// Benchmark: Construction from unordered_map (small)
static void BM_ConstructionFromMapSmall(benchmark::State& state) {
    std::unordered_map<int, int> data;
    for (int i = 0; i < 10; ++i) {
        data[i] = i + 1;
    }

    for (auto _ : state) {
        RouletteWheel<int, int> wheel(data);
        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_ConstructionFromMapSmall);

// Benchmark: Construction from unordered_map (medium)
static void BM_ConstructionFromMapMedium(benchmark::State& state) {
    std::unordered_map<int, int> data;
    for (int i = 0; i < 100; ++i) {
        data[i] = i + 1;
    }

    for (auto _ : state) {
        RouletteWheel<int, int> wheel(data);
        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_ConstructionFromMapMedium);

// Benchmark: Construction from unordered_map (large)
static void BM_ConstructionFromMapLarge(benchmark::State& state) {
    std::unordered_map<int, int> data;
    for (int i = 0; i < 1000; ++i) {
        data[i] = i + 1;
    }

    for (auto _ : state) {
        RouletteWheel<int, int> wheel(data);
        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_ConstructionFromMapLarge);

// Benchmark: Construction from vector of tuples (small)
static void BM_ConstructionFromVectorSmall(benchmark::State& state) {
    std::vector<std::tuple<int, int>> data;
    for (int i = 0; i < 10; ++i) {
        data.emplace_back(i, i + 1);
    }

    for (auto _ : state) {
        RouletteWheel<int, int> wheel(data);
        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_ConstructionFromVectorSmall);

// Benchmark: Construction from vector of tuples (medium)
static void BM_ConstructionFromVectorMedium(benchmark::State& state) {
    std::vector<std::tuple<int, int>> data;
    for (int i = 0; i < 100; ++i) {
        data.emplace_back(i, i + 1);
    }

    for (auto _ : state) {
        RouletteWheel<int, int> wheel(data);
        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_ConstructionFromVectorMedium);

// Benchmark: Construction from vector of tuples (large)
static void BM_ConstructionFromVectorLarge(benchmark::State& state) {
    std::vector<std::tuple<int, int>> data;
    for (int i = 0; i < 1000; ++i) {
        data.emplace_back(i, i + 1);
    }

    for (auto _ : state) {
        RouletteWheel<int, int> wheel(data);
        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_ConstructionFromVectorLarge);

// Benchmark: Construction from map with string elements
static void BM_ConstructionFromMapStrings(benchmark::State& state) {
    const int numElements = state.range(0);
    std::unordered_map<std::string, double> data;
    for (int i = 0; i < numElements; ++i) {
        data["Element_" + std::to_string(i)] = static_cast<double>(i + 1);
    }

    for (auto _ : state) {
        RouletteWheel<std::string, double> wheel(data);
        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_ConstructionFromMapStrings)->Range(10, 500);

// Benchmark: Copy construction
static void BM_CopyConstruction(benchmark::State& state) {
    const int numElements = state.range(0);
    RouletteWheel<int, int> original;
    for (int i = 0; i < numElements; ++i) {
        original.addRegion(i, i + 1);
    }

    for (auto _ : state) {
        RouletteWheel<int, int> copy = original;
        benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK(BM_CopyConstruction)->Range(10, 1000);

// Benchmark: Move construction
static void BM_MoveConstruction(benchmark::State& state) {
    const int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> original;
        for (int i = 0; i < numElements; ++i) {
            original.addRegion(i, i + 1);
        }
        state.ResumeTiming();

        RouletteWheel<int, int> moved = std::move(original);
        benchmark::DoNotOptimize(moved);
    }
}
BENCHMARK(BM_MoveConstruction)->Range(10, 1000);
