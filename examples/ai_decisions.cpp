#include "../RouletteWheel.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

// Represents an action an AI can take
struct Action {
    std::string name;
    std::string description;

    bool operator==(const Action& other) const {
        return name == other.name;
    }
};

// Hash for Action
namespace std {
    template<>
    struct hash<Action> {
        size_t operator()(const Action& action) const {
            return hash<string>()(action.name);
        }
    };
}

int main() {
    std::cout << "=== AI Decision Making with RouletteWheel ===\n\n";

    // Example 1: NPC Combat Behavior
    std::cout << "Example 1: NPC Combat AI\n";
    std::cout << "========================\n\n";

    RouletteWheel<Action, int> combatAI;
    combatAI.addRegion({"Attack", "Deal damage to enemy"}, 50);
    combatAI.addRegion({"Defend", "Block incoming attack"}, 30);
    combatAI.addRegion({"Heal", "Restore health"}, 15);
    combatAI.addRegion({"Special", "Use special ability"}, 5);

    std::cout << "NPC decision weights (Normal state):\n";
    for (const auto& region : combatAI.getRegions()) {
        std::cout << "  " << region.getElement().name << ": "
                  << combatAI.getSelectionProbability(region.getElement()) << "%\n";
    }

    std::cout << "\nSimulating 10 combat turns:\n";
    for (int turn = 1; turn <= 10; ++turn) {
        Action decision = combatAI.select();
        std::cout << "  Turn " << turn << ": " << decision.name
                  << " - " << decision.description << "\n";
    }

    // Example 2: Low Health - Defensive Behavior
    std::cout << "\n\nExample 2: Adaptive Combat AI (Low Health)\n";
    std::cout << "===========================================\n\n";

    RouletteWheel<Action, int> defensiveAI;
    defensiveAI.addRegion({"Attack", "Deal damage to enemy"}, 10);   // Reduced
    defensiveAI.addRegion({"Defend", "Block incoming attack"}, 40);  // Increased
    defensiveAI.addRegion({"Heal", "Restore health"}, 45);           // Greatly increased
    defensiveAI.addRegion({"Special", "Use special ability"}, 5);

    std::cout << "NPC decision weights (Low Health state):\n";
    for (const auto& region : defensiveAI.getRegions()) {
        std::cout << "  " << region.getElement().name << ": "
                  << defensiveAI.getSelectionProbability(region.getElement()) << "%\n";
    }

    std::cout << "\nSimulating 10 combat turns (low health):\n";
    for (int turn = 1; turn <= 10; ++turn) {
        Action decision = defensiveAI.select();
        std::cout << "  Turn " << turn << ": " << decision.name << "\n";
    }

    // Example 3: NPC Dialogue Responses
    std::cout << "\n\nExample 3: NPC Dialogue AI\n";
    std::cout << "===========================\n\n";

    RouletteWheel<std::string, int> dialogueAI;
    dialogueAI.addRegion("Friendly greeting", 40);
    dialogueAI.addRegion("Ask about quest", 25);
    dialogueAI.addRegion("Share rumor", 20);
    dialogueAI.addRegion("Complain about weather", 10);
    dialogueAI.addRegion("Mysterious hint", 5);

    std::cout << "NPC conversation over 5 interactions:\n";
    for (int i = 1; i <= 5; ++i) {
        std::string response = dialogueAI.select();
        std::cout << "  Interaction " << i << ": NPC says - \"" << response << "\"\n";
    }

    // Example 4: Creature AI - Behavior State Machine
    std::cout << "\n\nExample 4: Wildlife AI Behavior\n";
    std::cout << "================================\n\n";

    struct Behavior {
        std::string action;
        std::string state;

        bool operator==(const Behavior& other) const {
            return action == other.action;
        }
    };

    struct BehaviorHash {
        size_t operator()(const Behavior& b) const {
            return std::hash<std::string>{}(b.action);
        }
    };

    std::vector<std::tuple<Behavior, int>> peacefulBehaviors = {
        {{{"Graze", "Peaceful"}}, 50},
        {{{"Wander", "Peaceful"}}, 30},
        {{{"Rest", "Peaceful"}}, 15},
        {{{"Alert", "Peaceful"}}, 5}
    };

    RouletteWheel<Behavior, int> wildlifeAI(peacefulBehaviors);

    std::cout << "Deer AI in peaceful state (10 time steps):\n";
    for (int t = 1; t <= 10; ++t) {
        Behavior behavior = wildlifeAI.select();
        std::cout << "  t=" << t << ": " << behavior.action << "\n";
    }

    // Example 5: Boss AI with Phase Changes
    std::cout << "\n\nExample 5: Boss Fight AI (Multi-Phase)\n";
    std::cout << "=======================================\n\n";

    // Phase 1: Normal attacks
    std::cout << "Phase 1 (100% - 50% health):\n";
    RouletteWheel<std::string, int> bossPhase1;
    bossPhase1.addRegion("Slash", 60);
    bossPhase1.addRegion("Charge", 30);
    bossPhase1.addRegion("Roar", 10);

    std::cout << "  Boss actions (5 turns):\n";
    for (int i = 1; i <= 5; ++i) {
        std::cout << "    " << i << ". " << bossPhase1.select() << "\n";
    }

    // Phase 2: Enraged
    std::cout << "\nPhase 2 (50% - 25% health) - ENRAGED:\n";
    RouletteWheel<std::string, int> bossPhase2;
    bossPhase2.addRegion("Double Slash", 40);
    bossPhase2.addRegion("Berserker Charge", 35);
    bossPhase2.addRegion("Ground Slam", 20);
    bossPhase2.addRegion("Summon Adds", 5);

    std::cout << "  Boss actions (5 turns):\n";
    for (int i = 1; i <= 5; ++i) {
        std::cout << "    " << i << ". " << bossPhase2.select() << "\n";
    }

    // Phase 3: Desperate
    std::cout << "\nPhase 3 (<25% health) - DESPERATE:\n";
    RouletteWheel<std::string, int> bossPhase3;
    bossPhase3.addRegion("Desperate Flurry", 30);
    bossPhase3.addRegion("Life Drain", 30);
    bossPhase3.addRegion("Ultimate Attack", 25);
    bossPhase3.addRegion("Defensive Stance", 15);

    std::cout << "  Boss actions (5 turns):\n";
    for (int i = 1; i <= 5; ++i) {
        std::cout << "    " << i << ". " << bossPhase3.select() << "\n";
    }

    // Example 6: Strategic Resource Allocation AI
    std::cout << "\n\nExample 6: RTS AI Resource Allocation\n";
    std::cout << "======================================\n\n";

    RouletteWheel<std::string, int> rtsAI;
    rtsAI.addRegion("Build Workers", 30);
    rtsAI.addRegion("Build Army Units", 40);
    rtsAI.addRegion("Research Technology", 20);
    rtsAI.addRegion("Expand Base", 10);

    std::cout << "AI decisions over 15 game cycles:\n";
    std::map<std::string, int> resourceAllocation;

    for (int cycle = 1; cycle <= 15; ++cycle) {
        std::string decision = rtsAI.select();
        resourceAllocation[decision]++;

        if (cycle % 3 == 0) {
            std::cout << "  Cycle " << cycle << ": " << decision << "\n";
        }
    }

    std::cout << "\nResource allocation summary:\n";
    for (const auto& [decision, count] : resourceAllocation) {
        std::cout << "  " << decision << ": " << count << " times\n";
    }

    // Example 7: Procedural Quest Generation
    std::cout << "\n\nExample 7: Procedural Quest Generation\n";
    std::cout << "=======================================\n\n";

    RouletteWheel<std::string, int> questTypeWheel;
    questTypeWheel.addRegion("Kill Monsters", 35);
    questTypeWheel.addRegion("Collect Items", 30);
    questTypeWheel.addRegion("Escort NPC", 20);
    questTypeWheel.addRegion("Explore Location", 10);
    questTypeWheel.addRegion("Solve Puzzle", 5);

    RouletteWheel<std::string, int> questDifficultyWheel;
    questDifficultyWheel.addRegion("Easy", 50);
    questDifficultyWheel.addRegion("Medium", 35);
    questDifficultyWheel.addRegion("Hard", 12);
    questDifficultyWheel.addRegion("Epic", 3);

    std::cout << "Generating 5 random quests:\n";
    for (int i = 1; i <= 5; ++i) {
        std::string type = questTypeWheel.select();
        std::string difficulty = questDifficultyWheel.select();
        std::cout << "  Quest " << i << ": [" << difficulty << "] " << type << "\n";
    }

    // Example 8: Machine Learning Action Selection (Exploration vs Exploitation)
    std::cout << "\n\nExample 8: Reinforcement Learning Agent\n";
    std::cout << "=========================================\n\n";

    RouletteWheel<std::string, double> rlAgent;
    rlAgent.addRegion("Explore (Random Action)", 0.3);    // Epsilon = 30%
    rlAgent.addRegion("Exploit (Best Action)", 0.7);      // 1 - Epsilon = 70%

    std::cout << "Agent decisions over 20 steps:\n";
    std::map<std::string, int> strategyCounts;

    for (int step = 1; step <= 20; ++step) {
        std::string strategy = rlAgent.select();
        strategyCounts[strategy]++;

        if (step % 5 == 0) {
            std::cout << "  Step " << step << ": " << strategy << "\n";
        }
    }

    std::cout << "\nStrategy distribution:\n";
    for (const auto& [strategy, count] : strategyCounts) {
        std::cout << "  " << strategy << ": " << count << " / 20 ("
                  << (count * 100.0 / 20.0) << "%)\n";
    }

    std::cout << "\n=== End of AI Decision Making Examples ===\n";

    return 0;
}
