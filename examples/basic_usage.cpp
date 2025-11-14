#include "../RouletteWheel.hpp"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== RouletteWheel Library - Basic Usage Example ===\n\n";

    // Example 1: Creating a wheel with integers
    std::cout << "Example 1: Integer wheel\n";
    std::cout << "-------------------------\n";
    RouletteWheel<int, int> numberWheel;
    numberWheel.addRegion(1, 10);
    numberWheel.addRegion(2, 20);
    numberWheel.addRegion(3, 30);
    numberWheel.addRegion(4, 40);

    std::cout << "Number 1 probability: " << numberWheel.getSelectionProbability(1) << "%\n";
    std::cout << "Number 2 probability: " << numberWheel.getSelectionProbability(2) << "%\n";
    std::cout << "Number 3 probability: " << numberWheel.getSelectionProbability(3) << "%\n";
    std::cout << "Number 4 probability: " << numberWheel.getSelectionProbability(4) << "%\n";

    std::cout << "\nSelecting 5 numbers:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  Selected: " << numberWheel.select() << "\n";
    }

    // Example 2: Creating a wheel with strings
    std::cout << "\n\nExample 2: String wheel (Fruit selection)\n";
    std::cout << "-------------------------------------------\n";
    RouletteWheel<std::string, double> fruitWheel;
    fruitWheel.addRegion("Apple", 5.0);
    fruitWheel.addRegion("Banana", 3.0);
    fruitWheel.addRegion("Orange", 2.0);
    fruitWheel.addRegion("Mango", 1.0);

    std::cout << "Wheel size: " << fruitWheel.size() << " fruits\n\n";

    std::cout << "Selection probabilities:\n";
    for (const auto& region : fruitWheel.getRegions()) {
        std::cout << "  " << region.getElement() << ": "
                  << fruitWheel.getSelectionProbability(region.getElement()) << "%\n";
    }

    std::cout << "\nSelecting 10 fruits:\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << "  " << (i + 1) << ". " << fruitWheel.select() << "\n";
    }

    // Example 3: Using unordered_map constructor
    std::cout << "\n\nExample 3: Construction from unordered_map\n";
    std::cout << "-------------------------------------------\n";
    std::unordered_map<std::string, int> colorWeights = {
        {"Red", 25},
        {"Green", 25},
        {"Blue", 25},
        {"Yellow", 25}
    };

    RouletteWheel<std::string, int> colorWheel(colorWeights);
    std::cout << "Created wheel with " << colorWheel.size() << " colors\n";
    std::cout << "All colors have equal 25% chance\n\n";

    std::cout << "Drawing 5 colors:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  " << colorWheel.select() << "\n";
    }

    // Example 4: Select and remove
    std::cout << "\n\nExample 4: Select and remove (drawing without replacement)\n";
    std::cout << "-----------------------------------------------------------\n";
    RouletteWheel<std::string, int> prizeWheel;
    prizeWheel.addRegion("Gold Medal", 1);
    prizeWheel.addRegion("Silver Medal", 2);
    prizeWheel.addRegion("Bronze Medal", 3);
    prizeWheel.addRegion("Participation", 10);

    std::cout << "Initial wheel size: " << prizeWheel.size() << "\n\n";
    std::cout << "Drawing all prizes:\n";

    int position = 1;
    while (!prizeWheel.empty()) {
        std::string prize = prizeWheel.selectAndRemove();
        std::cout << "  Position " << position++ << ": " << prize
                  << " (remaining: " << prizeWheel.size() << ")\n";
    }

    // Example 5: Select and modify weight
    std::cout << "\n\nExample 5: Select and modify weight (decreasing probability)\n";
    std::cout << "-------------------------------------------------------------\n";
    RouletteWheel<std::string, int> deckWheel;
    deckWheel.addRegion("Card A", 10);
    deckWheel.addRegion("Card B", 10);
    deckWheel.addRegion("Card C", 10);

    std::cout << "Drawing 10 cards (each draw decreases weight by 2):\n";
    for (int i = 0; i < 10; ++i) {
        std::string card = deckWheel.selectAndModifyWeight(-2);
        std::cout << "  Draw " << (i + 1) << ": " << card
                  << " (wheel size: " << deckWheel.size() << ")\n";
    }

    // Example 6: Safe selection (returns optional)
    std::cout << "\n\nExample 6: Safe selection with optional\n";
    std::cout << "----------------------------------------\n";
    RouletteWheel<int, int> emptyWheel;

    auto result = emptyWheel.selectSafe();
    if (result.has_value()) {
        std::cout << "Selected: " << *result << "\n";
    } else {
        std::cout << "Wheel is empty - no selection made (safe!)\n";
    }

    emptyWheel.addRegion(42, 100);
    result = emptyWheel.selectSafe();
    if (result.has_value()) {
        std::cout << "After adding element - Selected: " << *result << "\n";
    }

    std::cout << "\n=== End of examples ===\n";

    return 0;
}
