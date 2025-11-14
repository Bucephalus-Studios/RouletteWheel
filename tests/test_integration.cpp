#include "../RouletteWheel.hpp"
#include <gtest/gtest.h>
#include <string>
#include <set>

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// Real-world scenario: Item rarity system
TEST_F(IntegrationTest, ItemRaritySystem) {
    enum class Rarity { Common, Uncommon, Rare, Epic, Legendary };

    std::unordered_map<Rarity, int> rarityWeights = {
        {Rarity::Common, 50},
        {Rarity::Uncommon, 30},
        {Rarity::Rare, 15},
        {Rarity::Epic, 4},
        {Rarity::Legendary, 1}
    };

    RouletteWheel<Rarity, int> lootBox(rarityWeights);

    // Count occurrences over many draws
    std::unordered_map<Rarity, int> counts;
    const int draws = 10000;

    for (int i = 0; i < draws; ++i) {
        Rarity item = lootBox.select();
        counts[item]++;
    }

    // Verify statistical distribution (with tolerance)
    EXPECT_NEAR((counts[Rarity::Common] * 100.0) / draws, 50.0, 2.0);
    EXPECT_NEAR((counts[Rarity::Uncommon] * 100.0) / draws, 30.0, 2.0);
    EXPECT_NEAR((counts[Rarity::Rare] * 100.0) / draws, 15.0, 2.0);
}

// Real-world scenario: Weighted shuffle with removal
TEST_F(IntegrationTest, WeightedShuffleWithRemoval) {
    RouletteWheel<std::string, int> playlist;
    playlist.addRegion("Song A", 5);
    playlist.addRegion("Song B", 3);
    playlist.addRegion("Song C", 2);
    playlist.addRegion("Song D", 1);

    std::vector<std::string> playOrder;

    while (!playlist.empty()) {
        std::string song = playlist.selectAndRemove();
        playOrder.push_back(song);
    }

    // Should have selected all 4 songs
    EXPECT_EQ(playOrder.size(), 4);

    // Verify all songs were played exactly once
    std::set<std::string> uniqueSongs(playOrder.begin(), playOrder.end());
    EXPECT_EQ(uniqueSongs.size(), 4);
}

// Real-world scenario: Dynamic probability adjustment
TEST_F(IntegrationTest, DynamicProbabilityAdjustment) {
    RouletteWheel<std::string, int> wheel;
    wheel.addRegion("Player 1", 10);
    wheel.addRegion("Player 2", 10);
    wheel.addRegion("Player 3", 10);

    // Simulate a game where winning decreases your chance
    for (int round = 0; round < 5; ++round) {
        std::string winner = wheel.selectAndModifyWeight(-2);
        // Winner's weight is decreased
        EXPECT_FALSE(wheel.empty());
    }

    // After 5 rounds with -2 each time, all players should still have weight
    EXPECT_EQ(wheel.size(), 3);
}

// Real-world scenario: Probability-based enemy spawning
TEST_F(IntegrationTest, EnemySpawningSystem) {
    struct Enemy {
        std::string type;
        int dangerLevel;

        bool operator==(const Enemy& other) const {
            return type == other.type;
        }
    };

    struct EnemyHash {
        size_t operator()(const Enemy& e) const {
            return std::hash<std::string>{}(e.type);
        }
    };

    std::vector<std::tuple<Enemy, int>> enemies = {
        {{"Goblin", 1}, 50},
        {{"Orc", 3}, 30},
        {{"Dragon", 10}, 5}
    };

    RouletteWheel<Enemy, int> spawner(enemies);

    // Spawn 1000 enemies for better statistical distribution
    int goblinCount = 0;
    int orcCount = 0;
    int dragonCount = 0;

    for (int i = 0; i < 1000; ++i) {
        Enemy spawned = spawner.select();
        if (spawned.type == "Goblin") goblinCount++;
        else if (spawned.type == "Orc") orcCount++;
        else if (spawned.type == "Dragon") dragonCount++;
    }

    // With 1000 samples and proper weights, we should see all types
    EXPECT_GT(goblinCount, 0);
    EXPECT_GT(orcCount, 0);
    EXPECT_GT(dragonCount, 0);
}

// Real-world scenario: Gacha system with pity system
TEST_F(IntegrationTest, GachaSystemWithPity) {
    RouletteWheel<int, int> gacha;

    // Star ratings: 3-star, 4-star, 5-star
    gacha.addRegion(3, 94);  // 94% chance
    gacha.addRegion(4, 5);   // 5% chance
    gacha.addRegion(5, 1);   // 1% chance

    int pulls = 0;
    int fiveStarPulls = 0;
    const int maxPullsWithoutFiveStar = 90;

    // Simulate pulling until we get a 5-star
    for (int i = 0; i < 1000; ++i) {
        pulls++;

        int result;
        if (pulls >= maxPullsWithoutFiveStar) {
            // Pity system: guarantee 5-star
            result = 5;
            pulls = 0;
        } else {
            result = gacha.select();
        }

        if (result == 5) {
            fiveStarPulls++;
            pulls = 0;
        }
    }

    // Should have gotten at least some 5-stars due to pity
    EXPECT_GT(fiveStarPulls, 0);
}

// Real-world scenario: Weighted task scheduler
TEST_F(IntegrationTest, WeightedTaskScheduler) {
    RouletteWheel<std::string, int> taskQueue;
    taskQueue.addRegion("HighPriority", 10);
    taskQueue.addRegion("MediumPriority", 5);
    taskQueue.addRegion("LowPriority", 1);

    std::unordered_map<std::string, int> executionCounts;

    // Execute 100 tasks
    for (int i = 0; i < 100; ++i) {
        std::string task = taskQueue.select();
        executionCounts[task]++;
    }

    // High priority should execute most often
    EXPECT_GT(executionCounts["HighPriority"], executionCounts["MediumPriority"]);
    EXPECT_GT(executionCounts["MediumPriority"], executionCounts["LowPriority"]);
}

// Real-world scenario: Building a deck with weighted card selection
TEST_F(IntegrationTest, DeckBuildingWithWeights) {
    RouletteWheel<std::string, double> cardPool;
    cardPool.addRegion("Common Card", 10.0);
    cardPool.addRegion("Rare Card", 3.0);
    cardPool.addRegion("Mythic Card", 0.5);

    std::vector<std::string> deck;
    const size_t deckSize = 30;

    // Build a deck
    for (size_t i = 0; i < deckSize; ++i) {
        deck.push_back(cardPool.select());
    }

    EXPECT_EQ(deck.size(), deckSize);

    // Count card types
    int commonCount = std::count(deck.begin(), deck.end(), "Common Card");
    int rareCount = std::count(deck.begin(), deck.end(), "Rare Card");
    int mythicCount = std::count(deck.begin(), deck.end(), "Mythic Card");

    // Most should be common
    EXPECT_GT(commonCount, rareCount);
    EXPECT_GE(commonCount + rareCount + mythicCount, static_cast<int>(deckSize));
}

// Edge case: Rapid add/remove operations
TEST_F(IntegrationTest, RapidAddRemoveOperations) {
    RouletteWheel<int, int> wheel;

    for (int iteration = 0; iteration < 100; ++iteration) {
        // Add multiple elements
        for (int i = 0; i < 10; ++i) {
            wheel.addRegion(i, i + 1);
        }

        EXPECT_EQ(wheel.size(), 10);

        // Remove half of them
        for (int i = 0; i < 5; ++i) {
            wheel.selectAndRemove();
        }

        EXPECT_EQ(wheel.size(), 5);

        // Clear the rest
        while (!wheel.empty()) {
            wheel.selectAndRemove();
        }

        EXPECT_TRUE(wheel.empty());
    }
}

// Edge case: All elements have equal weight
TEST_F(IntegrationTest, AllEqualWeights) {
    RouletteWheel<char, int> wheel;
    wheel.addRegion('A', 1);
    wheel.addRegion('B', 1);
    wheel.addRegion('C', 1);
    wheel.addRegion('D', 1);

    std::unordered_map<char, int> counts;
    const int iterations = 10000;

    for (int i = 0; i < iterations; ++i) {
        char result = wheel.select();
        counts[result]++;
    }

    // Each should be roughly 25% (with tolerance)
    for (const auto& [key, count] : counts) {
        double percentage = (count * 100.0) / iterations;
        EXPECT_NEAR(percentage, 25.0, 2.0);
    }
}

// Edge case: One element has overwhelming weight
TEST_F(IntegrationTest, OverwhelmingWeight) {
    RouletteWheel<std::string, int> wheel;
    wheel.addRegion("Almost Always", 9999);
    wheel.addRegion("Almost Never", 1);

    int almostAlwaysCount = 0;
    const int iterations = 1000;

    for (int i = 0; i < iterations; ++i) {
        if (wheel.select() == "Almost Always") {
            ++almostAlwaysCount;
        }
    }

    // Should be selected ~99.99% of the time
    EXPECT_GT(almostAlwaysCount, 990);
}

// Stress test: Many modifications
TEST_F(IntegrationTest, ManyModifications) {
    RouletteWheel<int, int> wheel;

    // Start with many elements
    for (int i = 0; i < 100; ++i) {
        wheel.addRegion(i, 50);  // 100 elements with weight 50 each
    }

    size_t initialSize = wheel.size();
    EXPECT_EQ(initialSize, 100);

    // Perform modifications - enough to remove some but not all elements
    // Average: each element selected ~10 times, reducing weight from 50 to ~40
    for (int i = 0; i < 1000 && !wheel.empty(); ++i) {
        wheel.selectAndModifyWeight(-1);
    }

    // The wheel should have fewer elements than we started with (some may have
    // been selected more often and removed), but should not be empty
    size_t finalSize = wheel.size();
    EXPECT_LE(finalSize, initialSize);  // Size should be <= initial size
    // With 100 elements and only 1000 modifications, we shouldn't remove all
    EXPECT_GT(finalSize, 50);  // Should have more than half remaining
}
