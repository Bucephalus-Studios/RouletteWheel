#include "../RouletteWheel.hpp"
#include <benchmark/benchmark.h>
#include <string>

// Benchmark: AddRegion to empty wheel
static void BM_AddRegionEmpty(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        state.ResumeTiming();

        wheel.addRegion(42, 100);

        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_AddRegionEmpty);

// Benchmark: AddRegion (new elements)
static void BM_AddRegionNew(benchmark::State& state) {
    const int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            wheel.addRegion(i, 100);
        }

        benchmark::DoNotOptimize(wheel);
    }

    state.SetItemsProcessed(state.iterations() * numElements);
}
BENCHMARK(BM_AddRegionNew)->Range(10, 1000);

// Benchmark: AddRegion (combining weights - worst case: always same element)
static void BM_AddRegionCombine(benchmark::State& state) {
    const int numAdds = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        state.ResumeTiming();

        for (int i = 0; i < numAdds; ++i) {
            wheel.addRegion(0, 1);  // Always adding to same element
        }

        benchmark::DoNotOptimize(wheel);
    }

    state.SetItemsProcessed(state.iterations() * numAdds);
}
BENCHMARK(BM_AddRegionCombine)->Range(10, 1000);

// Benchmark: AddRegion with string elements
static void BM_AddRegionStrings(benchmark::State& state) {
    const int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<std::string, int> wheel;
        std::vector<std::string> elements;
        for (int i = 0; i < numElements; ++i) {
            elements.push_back("Element_" + std::to_string(i));
        }
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            wheel.addRegion(elements[i], 100);
        }

        benchmark::DoNotOptimize(wheel);
    }

    state.SetItemsProcessed(state.iterations() * numElements);
}
BENCHMARK(BM_AddRegionStrings)->Range(10, 500);

// Benchmark: RemoveElement (first element)
static void BM_RemoveElementFirst(benchmark::State& state) {
    const int numElements = 100;

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        for (int i = 0; i < numElements; ++i) {
            wheel.addRegion(i, 100);
        }
        state.ResumeTiming();

        wheel.removeElement(0);

        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_RemoveElementFirst);

// Benchmark: RemoveElement (middle element)
static void BM_RemoveElementMiddle(benchmark::State& state) {
    const int numElements = 100;

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        for (int i = 0; i < numElements; ++i) {
            wheel.addRegion(i, 100);
        }
        state.ResumeTiming();

        wheel.removeElement(numElements / 2);

        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_RemoveElementMiddle);

// Benchmark: RemoveElement (last element)
static void BM_RemoveElementLast(benchmark::State& state) {
    const int numElements = 100;

    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        for (int i = 0; i < numElements; ++i) {
            wheel.addRegion(i, 100);
        }
        state.ResumeTiming();

        wheel.removeElement(numElements - 1);

        benchmark::DoNotOptimize(wheel);
    }
}
BENCHMARK(BM_RemoveElementLast);

// Benchmark: RemoveElement (non-existent)
static void BM_RemoveElementNonExistent(benchmark::State& state) {
    const int numElements = 100;
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < numElements; ++i) {
        wheel.addRegion(i, 100);
    }

    for (auto _ : state) {
        bool result = wheel.removeElement(9999);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_RemoveElementNonExistent);

// Benchmark: RemoveInvalidRegions (no invalid)
static void BM_RemoveInvalidRegionsNone(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 100; ++i) {
        wheel.addRegion(i, 100);
    }

    for (auto _ : state) {
        size_t removed = wheel.removeInvalidRegions();
        benchmark::DoNotOptimize(removed);
    }
}
BENCHMARK(BM_RemoveInvalidRegionsNone);

// Benchmark: GetSelectionProbability (element exists)
static void BM_GetSelectionProbabilityExists(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 100; ++i) {
        wheel.addRegion(i, i + 1);
    }

    for (auto _ : state) {
        double prob = wheel.getSelectionProbability(50);
        benchmark::DoNotOptimize(prob);
    }
}
BENCHMARK(BM_GetSelectionProbabilityExists);

// Benchmark: GetSelectionProbability (element doesn't exist)
static void BM_GetSelectionProbabilityNotExists(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 100; ++i) {
        wheel.addRegion(i, i + 1);
    }

    for (auto _ : state) {
        double prob = wheel.getSelectionProbability(9999);
        benchmark::DoNotOptimize(prob);
    }
}
BENCHMARK(BM_GetSelectionProbabilityNotExists);

// Benchmark: Empty check
static void BM_Empty(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    wheel.addRegion(1, 100);

    for (auto _ : state) {
        bool empty = wheel.empty();
        benchmark::DoNotOptimize(empty);
    }
}
BENCHMARK(BM_Empty);

// Benchmark: Size check
static void BM_Size(benchmark::State& state) {
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < 100; ++i) {
        wheel.addRegion(i, 100);
    }

    for (auto _ : state) {
        size_t size = wheel.size();
        benchmark::DoNotOptimize(size);
    }
}
BENCHMARK(BM_Size);

// Benchmark: GetRegions
static void BM_GetRegions(benchmark::State& state) {
    const int numElements = state.range(0);
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < numElements; ++i) {
        wheel.addRegion(i, 100);
    }

    for (auto _ : state) {
        const auto& regions = wheel.getRegions();
        benchmark::DoNotOptimize(regions);
    }
}
BENCHMARK(BM_GetRegions)->Range(10, 1000);

// Benchmark: Interleaved add and select operations
static void BM_InterleavedAddSelect(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        state.ResumeTiming();

        for (int i = 0; i < 100; ++i) {
            wheel.addRegion(i, 100);
            if (i % 10 == 0 && i > 0) {
                benchmark::DoNotOptimize(wheel.select());
            }
        }
    }
}
BENCHMARK(BM_InterleavedAddSelect);

// Benchmark: Interleaved add and remove operations
static void BM_InterleavedAddRemove(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        RouletteWheel<int, int> wheel;
        state.ResumeTiming();

        for (int i = 0; i < 100; ++i) {
            wheel.addRegion(i, 100);
            if (i % 10 == 0 && i > 0) {
                wheel.removeElement(i - 5);
            }
        }
    }
}
BENCHMARK(BM_InterleavedAddRemove);

// Benchmark: Sequential operations (real-world simulation)
static void BM_RealWorldUsage(benchmark::State& state) {
    for (auto _ : state) {
        RouletteWheel<std::string, int> wheel;

        // Initial setup
        wheel.addRegion("Common", 50);
        wheel.addRegion("Uncommon", 30);
        wheel.addRegion("Rare", 15);
        wheel.addRegion("Epic", 4);
        wheel.addRegion("Legendary", 1);

        // Perform typical operations
        for (int i = 0; i < 100; ++i) {
            std::string selected = wheel.select();
            double prob = wheel.getSelectionProbability(selected);
            benchmark::DoNotOptimize(selected);
            benchmark::DoNotOptimize(prob);
        }
    }
}
BENCHMARK(BM_RealWorldUsage);

// Benchmark: Memory access patterns (cache effects)
static void BM_CacheEffects(benchmark::State& state) {
    const int numElements = state.range(0);
    RouletteWheel<int, int> wheel;
    for (int i = 0; i < numElements; ++i) {
        wheel.addRegion(i, 1);
    }

    for (auto _ : state) {
        // Access all elements sequentially via probability queries
        for (int i = 0; i < numElements; ++i) {
            double prob = wheel.getSelectionProbability(i);
            benchmark::DoNotOptimize(prob);
        }
    }

    state.SetItemsProcessed(state.iterations() * numElements);
}
BENCHMARK(BM_CacheEffects)->Range(8, 2048);
