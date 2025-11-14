#include "../RouletteWheel.hpp"
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <vector>

class RouletteWheelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a wheel with predictable random seed for testing
        wheel.seedRandom(42);
    }

    RouletteWheel<std::string, int> wheel;
};

// Constructor Tests
TEST_F(RouletteWheelTest, DefaultConstructor) {
    RouletteWheel<int, double> emptyWheel;
    EXPECT_TRUE(emptyWheel.empty());
    EXPECT_EQ(emptyWheel.size(), 0);
}

TEST_F(RouletteWheelTest, UnorderedMapConstructor) {
    std::unordered_map<std::string, int> data = {
        {"apple", 3},
        {"banana", 2},
        {"cherry", 5}
    };

    RouletteWheel<std::string, int> wheel(data);

    EXPECT_FALSE(wheel.empty());
    EXPECT_EQ(wheel.size(), 3);
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("apple"), 30.0);
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("banana"), 20.0);
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("cherry"), 50.0);
}

TEST_F(RouletteWheelTest, VectorOfTuplesConstructor) {
    std::vector<std::tuple<int, double>> data = {
        {1, 1.0},
        {2, 2.0},
        {3, 3.0}
    };

    RouletteWheel<int, double> wheel(data);

    EXPECT_FALSE(wheel.empty());
    EXPECT_EQ(wheel.size(), 3);
}

// Add Region Tests
TEST_F(RouletteWheelTest, AddSingleRegion) {
    wheel.addRegion("test", 10);
    EXPECT_EQ(wheel.size(), 1);
    EXPECT_FALSE(wheel.empty());
}

TEST_F(RouletteWheelTest, AddMultipleRegions) {
    wheel.addRegion("first", 5);
    wheel.addRegion("second", 10);
    wheel.addRegion("third", 15);

    EXPECT_EQ(wheel.size(), 3);
}

TEST_F(RouletteWheelTest, AddRegionCombinesWeights) {
    wheel.addRegion("item", 5);
    wheel.addRegion("item", 3);

    EXPECT_EQ(wheel.size(), 1);
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("item"), 100.0);
}

TEST_F(RouletteWheelTest, AddRegionThrowsOnZeroWeight) {
    EXPECT_THROW(wheel.addRegion("invalid", 0), std::invalid_argument);
}

TEST_F(RouletteWheelTest, AddRegionThrowsOnNegativeWeight) {
    EXPECT_THROW(wheel.addRegion("invalid", -5), std::invalid_argument);
}

// Selection Tests
TEST_F(RouletteWheelTest, SelectThrowsOnEmptyWheel) {
    EXPECT_THROW(wheel.select(), std::runtime_error);
}

TEST_F(RouletteWheelTest, SelectSafeReturnsNulloptOnEmptyWheel) {
    auto result = wheel.selectSafe();
    EXPECT_FALSE(result.has_value());
}

TEST_F(RouletteWheelTest, SelectSingleElement) {
    wheel.addRegion("only", 100);
    EXPECT_EQ(wheel.select(), "only");
}

TEST_F(RouletteWheelTest, SelectFromMultipleElements) {
    wheel.addRegion("a", 1);
    wheel.addRegion("b", 1);
    wheel.addRegion("c", 1);

    // Just verify it doesn't crash and returns one of the elements
    std::string result = wheel.select();
    EXPECT_TRUE(result == "a" || result == "b" || result == "c");
}

TEST_F(RouletteWheelTest, SelectDistribution) {
    // Create a wheel with known probabilities
    RouletteWheel<std::string, int> testWheel;
    testWheel.seedRandom(42);
    testWheel.addRegion("common", 90);
    testWheel.addRegion("rare", 10);

    // Run many selections to verify statistical distribution
    const int iterations = 10000;
    int commonCount = 0;
    int rareCount = 0;

    for (int i = 0; i < iterations; ++i) {
        std::string result = testWheel.select();
        if (result == "common") {
            ++commonCount;
        } else if (result == "rare") {
            ++rareCount;
        }
    }

    // Expected: ~90% common, ~10% rare (with some tolerance)
    double commonPercent = (commonCount * 100.0) / iterations;
    double rarePercent = (rareCount * 100.0) / iterations;

    EXPECT_NEAR(commonPercent, 90.0, 2.0);  // Within 2% tolerance
    EXPECT_NEAR(rarePercent, 10.0, 2.0);
}

// Select and Modify Weight Tests
TEST_F(RouletteWheelTest, SelectAndModifyWeightDecreasesWeight) {
    wheel.addRegion("item", 10);
    wheel.selectAndModifyWeight(-5);

    EXPECT_EQ(wheel.size(), 1);
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("item"), 100.0);
}

TEST_F(RouletteWheelTest, SelectAndModifyWeightRemovesWhenZero) {
    wheel.addRegion("item", 5);
    wheel.selectAndModifyWeight(-5);

    EXPECT_TRUE(wheel.empty());
}

TEST_F(RouletteWheelTest, SelectAndModifyWeightRemovesWhenNegative) {
    wheel.addRegion("item", 3);
    wheel.selectAndModifyWeight(-10);

    EXPECT_TRUE(wheel.empty());
}

TEST_F(RouletteWheelTest, SelectAndModifyWeightIncreasesWeight) {
    wheel.addRegion("item", 10);
    wheel.selectAndModifyWeight(5);

    EXPECT_EQ(wheel.size(), 1);
}

// Select and Remove Tests
TEST_F(RouletteWheelTest, SelectAndRemoveSingleElement) {
    wheel.addRegion("item", 100);
    std::string result = wheel.selectAndRemove();

    EXPECT_EQ(result, "item");
    EXPECT_TRUE(wheel.empty());
}

TEST_F(RouletteWheelTest, SelectAndRemoveMultipleElements) {
    wheel.addRegion("a", 1);
    wheel.addRegion("b", 1);
    wheel.addRegion("c", 1);

    EXPECT_EQ(wheel.size(), 3);
    wheel.selectAndRemove();
    EXPECT_EQ(wheel.size(), 2);
    wheel.selectAndRemove();
    EXPECT_EQ(wheel.size(), 1);
    wheel.selectAndRemove();
    EXPECT_TRUE(wheel.empty());
}

// Remove Element Tests
TEST_F(RouletteWheelTest, RemoveElementExisting) {
    wheel.addRegion("item", 10);
    bool removed = wheel.removeElement("item");

    EXPECT_TRUE(removed);
    EXPECT_TRUE(wheel.empty());
}

TEST_F(RouletteWheelTest, RemoveElementNonExisting) {
    wheel.addRegion("item", 10);
    bool removed = wheel.removeElement("nonexistent");

    EXPECT_FALSE(removed);
    EXPECT_EQ(wheel.size(), 1);
}

// Remove Invalid Regions Tests
TEST_F(RouletteWheelTest, RemoveInvalidRegionsNoneInvalid) {
    wheel.addRegion("a", 5);
    wheel.addRegion("b", 10);

    size_t removed = wheel.removeInvalidRegions();

    EXPECT_EQ(removed, 0);
    EXPECT_EQ(wheel.size(), 2);
}

// Query Tests
TEST_F(RouletteWheelTest, EmptyReturnsTrue) {
    EXPECT_TRUE(wheel.empty());
}

TEST_F(RouletteWheelTest, EmptyReturnsFalse) {
    wheel.addRegion("item", 5);
    EXPECT_FALSE(wheel.empty());
}

TEST_F(RouletteWheelTest, SizeReturnsCorrectCount) {
    EXPECT_EQ(wheel.size(), 0);
    wheel.addRegion("a", 1);
    EXPECT_EQ(wheel.size(), 1);
    wheel.addRegion("b", 2);
    EXPECT_EQ(wheel.size(), 2);
}

TEST_F(RouletteWheelTest, GetSelectionProbabilityCorrectCalculation) {
    wheel.addRegion("a", 25);
    wheel.addRegion("b", 25);
    wheel.addRegion("c", 50);

    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("a"), 25.0);
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("b"), 25.0);
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("c"), 50.0);
}

TEST_F(RouletteWheelTest, GetSelectionProbabilityNonExistent) {
    wheel.addRegion("a", 100);
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("nonexistent"), 0.0);
}

TEST_F(RouletteWheelTest, GetSelectionProbabilityEmptyWheel) {
    EXPECT_DOUBLE_EQ(wheel.getSelectionProbability("anything"), 0.0);
}

TEST_F(RouletteWheelTest, GetRegionsReturnsCorrectData) {
    wheel.addRegion("a", 10);
    wheel.addRegion("b", 20);

    const auto& regions = wheel.getRegions();
    EXPECT_EQ(regions.size(), 2);
}

// Edge Cases
TEST_F(RouletteWheelTest, FloatingPointWeights) {
    RouletteWheel<std::string, double> floatWheel;
    floatWheel.addRegion("a", 0.1);
    floatWheel.addRegion("b", 0.2);
    floatWheel.addRegion("c", 0.7);

    EXPECT_NEAR(floatWheel.getSelectionProbability("a"), 10.0, 0.01);
    EXPECT_NEAR(floatWheel.getSelectionProbability("b"), 20.0, 0.01);
    EXPECT_NEAR(floatWheel.getSelectionProbability("c"), 70.0, 0.01);
}

TEST_F(RouletteWheelTest, IntegerWeights) {
    RouletteWheel<int, int> intWheel;
    intWheel.addRegion(1, 1);
    intWheel.addRegion(2, 2);
    intWheel.addRegion(3, 7);

    EXPECT_DOUBLE_EQ(intWheel.getSelectionProbability(1), 10.0);
    EXPECT_DOUBLE_EQ(intWheel.getSelectionProbability(2), 20.0);
    EXPECT_DOUBLE_EQ(intWheel.getSelectionProbability(3), 70.0);
}

TEST_F(RouletteWheelTest, LargeNumberOfElements) {
    RouletteWheel<int, int> largeWheel;

    for (int i = 0; i < 1000; ++i) {
        largeWheel.addRegion(i, 1);
    }

    EXPECT_EQ(largeWheel.size(), 1000);
    EXPECT_NEAR(largeWheel.getSelectionProbability(0), 0.1, 0.01);

    // Verify selection works
    int result = largeWheel.select();
    EXPECT_GE(result, 0);
    EXPECT_LT(result, 1000);
}

TEST_F(RouletteWheelTest, SeedRandomProducesDifferentResults) {
    RouletteWheel<int, int> wheel1;
    RouletteWheel<int, int> wheel2;

    wheel1.seedRandom(12345);
    wheel2.seedRandom(67890);

    for (int i = 0; i < 10; ++i) {
        wheel1.addRegion(i, 1);
        wheel2.addRegion(i, 1);
    }

    // With different seeds, we expect different sequences
    std::vector<int> results1;
    std::vector<int> results2;

    for (int i = 0; i < 100; ++i) {
        results1.push_back(wheel1.select());
        results2.push_back(wheel2.select());
    }

    // Results should differ (statistically very unlikely to be identical)
    EXPECT_NE(results1, results2);
}

TEST_F(RouletteWheelTest, SeedRandomProducesConsistentResults) {
    RouletteWheel<int, int> wheel1;
    RouletteWheel<int, int> wheel2;

    wheel1.seedRandom(42);
    wheel2.seedRandom(42);

    for (int i = 0; i < 10; ++i) {
        wheel1.addRegion(i, 1);
        wheel2.addRegion(i, 1);
    }

    // With same seed, we expect same sequence
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(wheel1.select(), wheel2.select());
    }
}
