#include "../RouletteWheel.hpp"
#include <iostream>
#include <string>
#include <map>

// Represents an item that can be looted
struct Item {
    std::string name;
    std::string rarity;
    int value;

    bool operator==(const Item& other) const {
        return name == other.name;
    }
};

// Hash function for Item (needed for unordered_map)
namespace std {
    template<>
    struct hash<Item> {
        size_t operator()(const Item& item) const {
            return hash<string>()(item.name);
        }
    };
}

int main() {
    std::cout << "=== Game Loot System Example ===\n\n";

    // Create a loot table for a treasure chest
    std::cout << "Opening a Treasure Chest...\n";
    std::cout << "============================\n\n";

    RouletteWheel<Item, int> treasureChest;

    // Common items (50% total)
    treasureChest.addRegion({"Rusty Sword", "Common", 10}, 20);
    treasureChest.addRegion({"Wooden Shield", "Common", 15}, 15);
    treasureChest.addRegion({"Health Potion", "Common", 25}, 15);

    // Uncommon items (30% total)
    treasureChest.addRegion({"Iron Sword", "Uncommon", 50}, 15);
    treasureChest.addRegion({"Steel Shield", "Uncommon", 75}, 10);
    treasureChest.addRegion({"Mana Potion", "Uncommon", 40}, 5);

    // Rare items (15% total)
    treasureChest.addRegion({"Enchanted Bow", "Rare", 150}, 8);
    treasureChest.addRegion({"Magic Ring", "Rare", 200}, 7);

    // Epic items (4% total)
    treasureChest.addRegion({"Dragon Sword", "Epic", 500}, 3);
    treasureChest.addRegion({"Phoenix Armor", "Epic", 600}, 1);

    // Legendary item (1% total)
    treasureChest.addRegion({"Excalibur", "Legendary", 2000}, 1);

    // Display loot probabilities
    std::cout << "Loot Table Probabilities:\n";
    std::cout << "-------------------------\n";

    std::map<std::string, double> rarityProbabilities;
    for (const auto& region : treasureChest.getRegions()) {
        const Item& item = region.getElement();
        double prob = treasureChest.getSelectionProbability(item);
        std::cout << "  [" << item.rarity << "] " << item.name
                  << " - " << prob << "% (Value: " << item.value << " gold)\n";

        rarityProbabilities[item.rarity] += prob;
    }

    std::cout << "\nRarity Distribution:\n";
    std::cout << "--------------------\n";
    for (const auto& [rarity, prob] : rarityProbabilities) {
        std::cout << "  " << rarity << ": " << prob << "%\n";
    }

    // Simulate opening 100 chests
    std::cout << "\n\nSimulating 100 Treasure Chest Openings:\n";
    std::cout << "========================================\n";

    std::map<std::string, int> itemCounts;
    std::map<std::string, int> rarityCounts;
    int totalValue = 0;

    for (int i = 0; i < 100; ++i) {
        Item loot = treasureChest.select();
        itemCounts[loot.name]++;
        rarityCounts[loot.rarity]++;
        totalValue += loot.value;
    }

    std::cout << "\nItems Obtained:\n";
    std::cout << "---------------\n";
    for (const auto& [itemName, count] : itemCounts) {
        std::cout << "  " << itemName << ": " << count << "x\n";
    }

    std::cout << "\nRarity Distribution (Actual):\n";
    std::cout << "-----------------------------\n";
    for (const auto& [rarity, count] : rarityCounts) {
        std::cout << "  " << rarity << ": " << count << "% (expected ~"
                  << rarityProbabilities[rarity] << "%)\n";
    }

    std::cout << "\nTotal Value: " << totalValue << " gold\n";
    std::cout << "Average Value per Chest: " << (totalValue / 100.0) << " gold\n";

    // Simulate a boss encounter with guaranteed rare+ loot
    std::cout << "\n\n=== Boss Encounter Loot ===\n";
    std::cout << "===========================\n\n";

    RouletteWheel<Item, int> bossLoot;

    // Boss only drops rare or better items
    bossLoot.addRegion({"Dragon Scale Armor", "Rare", 300}, 50);
    bossLoot.addRegion({"Flaming Greatsword", "Rare", 350}, 30);
    bossLoot.addRegion({"Legendary Amulet", "Epic", 800}, 15);
    bossLoot.addRegion({"Crown of Kings", "Legendary", 2500}, 5);

    std::cout << "Defeating the boss 10 times:\n";
    std::cout << "----------------------------\n";

    for (int i = 1; i <= 10; ++i) {
        Item drop = bossLoot.select();
        std::cout << "  Kill " << i << ": [" << drop.rarity << "] "
                  << drop.name << " (" << drop.value << " gold)\n";
    }

    // Gacha system with pity mechanic
    std::cout << "\n\n=== Gacha System with Pity ===\n";
    std::cout << "==============================\n\n";

    RouletteWheel<std::string, int> gachaWheel;
    gachaWheel.addRegion("Common Character", 70);
    gachaWheel.addRegion("Rare Character", 25);
    gachaWheel.addRegion("SSR Character", 5);

    const int pityThreshold = 10;  // Guaranteed SSR after 10 pulls without one
    int pullsSinceSSR = 0;
    int totalPulls = 50;
    int ssrCount = 0;

    std::cout << "Performing " << totalPulls << " gacha pulls (Pity at " << pityThreshold << " pulls):\n";
    std::cout << "--------------------------------------------------------------------\n";

    for (int i = 1; i <= totalPulls; ++i) {
        pullsSinceSSR++;

        std::string result;
        if (pullsSinceSSR >= pityThreshold) {
            result = "SSR Character";
            pullsSinceSSR = 0;
            std::cout << "  Pull " << i << ": " << result << " [PITY ACTIVATED!]\n";
        } else {
            result = gachaWheel.select();
            if (result == "SSR Character") {
                pullsSinceSSR = 0;
                ssrCount++;
                std::cout << "  Pull " << i << ": " << result << " [LUCKY!]\n";
            }
        }

        if (result == "SSR Character") {
            ssrCount++;
        }
    }

    std::cout << "\nTotal SSR Characters obtained: " << ssrCount << " / " << totalPulls
              << " (" << (ssrCount * 100.0 / totalPulls) << "%)\n";

    std::cout << "\n=== End of Loot System Example ===\n";

    return 0;
}
