#include "../RouletteWheel.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

int main() {
    std::cout << "=== Weighted Random Selection Examples ===\n\n";

    // Example 1: Load Balancing with Server Weights
    std::cout << "Example 1: Load Balancing\n";
    std::cout << "=========================\n\n";

    RouletteWheel<std::string, int> serverPool;
    serverPool.addRegion("Server-A (High-Spec)", 50);    // 50% of traffic
    serverPool.addRegion("Server-B (Medium-Spec)", 30);  // 30% of traffic
    serverPool.addRegion("Server-C (Low-Spec)", 20);     // 20% of traffic

    std::cout << "Server weights configured:\n";
    for (const auto& region : serverPool.getRegions()) {
        std::cout << "  " << region.getElement() << ": "
                  << serverPool.getSelectionProbability(region.getElement()) << "%\n";
    }

    std::cout << "\nRouting 100 requests:\n";
    std::map<std::string, int> requestCounts;

    for (int i = 0; i < 100; ++i) {
        std::string server = serverPool.select();
        requestCounts[server]++;
    }

    for (const auto& [server, count] : requestCounts) {
        std::cout << "  " << server << ": " << count << " requests\n";
    }

    // Example 2: Random Event Generation in Games
    std::cout << "\n\nExample 2: Random Weather System\n";
    std::cout << "==================================\n\n";

    std::vector<std::tuple<std::string, int>> weatherData = {
        {"Sunny", 40},
        {"Cloudy", 30},
        {"Rainy", 20},
        {"Stormy", 8},
        {"Snowy", 2}
    };

    RouletteWheel<std::string, int> weatherWheel(weatherData);

    std::cout << "Simulating 30 days of weather:\n";
    std::map<std::string, int> weatherCounts;

    for (int day = 1; day <= 30; ++day) {
        std::string weather = weatherWheel.select();
        weatherCounts[weather]++;

        if (day % 10 == 0) {
            std::cout << "  Day " << day << ": " << weather << "\n";
        }
    }

    std::cout << "\nWeather statistics:\n";
    for (const auto& [weather, days] : weatherCounts) {
        double percentage = (days * 100.0) / 30.0;
        std::cout << "  " << weather << ": " << days << " days ("
                  << percentage << "%)\n";
    }

    // Example 3: A/B/C Testing with Different Variant Weights
    std::cout << "\n\nExample 3: A/B/C Testing\n";
    std::cout << "========================\n\n";

    RouletteWheel<std::string, double> abTestWheel;
    abTestWheel.addRegion("Variant A (Control)", 0.5);      // 50%
    abTestWheel.addRegion("Variant B (Test 1)", 0.3);       // 30%
    abTestWheel.addRegion("Variant C (Test 2)", 0.2);       // 20%

    std::cout << "Assigning 1000 users to variants:\n";
    std::map<std::string, int> variantAssignments;

    for (int i = 0; i < 1000; ++i) {
        std::string variant = abTestWheel.select();
        variantAssignments[variant]++;
    }

    for (const auto& [variant, count] : variantAssignments) {
        std::cout << "  " << variant << ": " << count << " users ("
                  << (count / 10.0) << "%)\n";
    }

    // Example 4: Weighted Random Playlist
    std::cout << "\n\nExample 4: Weighted Music Playlist\n";
    std::cout << "===================================\n\n";

    RouletteWheel<std::string, int> playlist;
    playlist.addRegion("Pop Song", 5);
    playlist.addRegion("Rock Song", 4);
    playlist.addRegion("Jazz Song", 2);
    playlist.addRegion("Classical Song", 1);

    std::cout << "Playing 20 songs (weighted by preference):\n";
    for (int i = 1; i <= 20; ++i) {
        std::string song = playlist.select();
        if (i % 5 == 0) {
            std::cout << "  " << i << ". " << song << "\n";
        }
    }

    // Example 5: Dynamic Weight Adjustment (Adaptive System)
    std::cout << "\n\nExample 5: Adaptive Enemy Spawning\n";
    std::cout << "===================================\n\n";

    RouletteWheel<std::string, int> enemySpawner;
    enemySpawner.addRegion("Goblin", 10);
    enemySpawner.addRegion("Orc", 10);
    enemySpawner.addRegion("Troll", 10);

    std::cout << "Initial spawn probabilities (equal):\n";
    for (const auto& region : enemySpawner.getRegions()) {
        std::cout << "  " << region.getElement() << ": "
                  << enemySpawner.getSelectionProbability(region.getElement()) << "%\n";
    }

    std::cout << "\nSpawning 10 enemies (each spawn reduces that enemy's weight):\n";
    for (int i = 1; i <= 10; ++i) {
        std::string enemy = enemySpawner.selectAndModifyWeight(-2);
        std::cout << "  Wave " << i << ": " << enemy << " spawned\n";

        if (enemySpawner.empty()) {
            std::cout << "  All enemy types exhausted!\n";
            break;
        }
    }

    // Example 6: Lottery System
    std::cout << "\n\nExample 6: Lottery Drawing (without replacement)\n";
    std::cout << "=================================================\n\n";

    RouletteWheel<int, double> lotteryBalls;
    for (int i = 1; i <= 50; ++i) {
        lotteryBalls.addRegion(i, 1.0);  // All balls have equal weight
    }

    std::cout << "Drawing 6 lottery numbers:\n  ";
    for (int i = 0; i < 6; ++i) {
        int ball = lotteryBalls.selectAndRemove();
        std::cout << ball;
        if (i < 5) std::cout << ", ";
    }
    std::cout << "\n";

    std::cout << "\nBalls remaining in drum: " << lotteryBalls.size() << "\n";

    // Example 7: Skill Tree Point Distribution (AI)
    std::cout << "\n\nExample 7: AI Skill Point Distribution\n";
    std::cout << "=======================================\n\n";

    RouletteWheel<std::string, int> skillWheel;
    skillWheel.addRegion("Strength", 3);
    skillWheel.addRegion("Dexterity", 3);
    skillWheel.addRegion("Intelligence", 2);
    skillWheel.addRegion("Vitality", 2);

    std::map<std::string, int> skillPoints;
    const int totalPoints = 20;

    std::cout << "AI distributing " << totalPoints << " skill points:\n";
    for (int i = 0; i < totalPoints; ++i) {
        std::string skill = skillWheel.select();
        skillPoints[skill]++;
    }

    std::cout << "\nFinal skill distribution:\n";
    for (const auto& [skill, points] : skillPoints) {
        std::cout << "  " << skill << ": " << points << " points\n";
    }

    // Example 8: Weighted Round-Robin Task Scheduling
    std::cout << "\n\nExample 8: Task Priority Scheduling\n";
    std::cout << "====================================\n\n";

    RouletteWheel<std::string, int> taskScheduler;
    taskScheduler.addRegion("Critical Task", 10);
    taskScheduler.addRegion("High Priority Task", 6);
    taskScheduler.addRegion("Medium Priority Task", 3);
    taskScheduler.addRegion("Low Priority Task", 1);

    std::cout << "Scheduling 20 task executions:\n";
    std::map<std::string, int> taskExecutions;

    for (int i = 1; i <= 20; ++i) {
        std::string task = taskScheduler.select();
        taskExecutions[task]++;
    }

    std::cout << "\nExecution counts:\n";
    for (const auto& [task, executions] : taskExecutions) {
        std::cout << "  " << task << ": " << executions << " times\n";
    }

    std::cout << "\n=== End of Weighted Selection Examples ===\n";

    return 0;
}
