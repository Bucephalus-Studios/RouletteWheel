#ifndef ROULETTE_WHEEL_HPP
#define ROULETTE_WHEEL_HPP

#include "classes/WheelRegion.hpp"
#include <vector>
#include <unordered_map>
#include <tuple>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <optional>

/**
 * @brief A weighted random selection data structure using the roulette wheel algorithm.
 *
 * This class implements the roulette wheel selection algorithm, commonly used in
 * genetic algorithms, game development, and probability-based systems. Elements
 * are selected randomly with probability proportional to their weights.
 *
 * @tparam E Element type to store
 * @tparam W Weight type (must be numeric: int, float, double, etc.)
 */
template<typename E, typename W>
class RouletteWheel {
public:
    /*** Constructors ***/

    /**
     * @brief Default constructor - creates an empty roulette wheel
     */
    RouletteWheel()
        : m_randomEngine(std::random_device{}()) {
    }

    /**
     * @brief Constructs a roulette wheel from an unordered map
     * @param elementWeightMap Map where keys are elements and values are weights
     */
    explicit RouletteWheel(const std::unordered_map<E, W>& elementWeightMap)
        : m_randomEngine(std::random_device{}()) {
        for (const auto& [element, weight] : elementWeightMap) {
            addRegion(element, weight);
        }
    }

    /**
     * @brief Constructs a roulette wheel from a vector of element-weight tuples
     * @param elementWeightPairs Vector of (element, weight) tuples
     */
    explicit RouletteWheel(const std::vector<std::tuple<E, W>>& elementWeightPairs)
        : m_randomEngine(std::random_device{}()) {
        for (const auto& [element, weight] : elementWeightPairs) {
            addRegion(element, weight);
        }
    }

    /*** Selection Methods ***/

    /**
     * @brief Selects an element using weighted random selection
     * @return The selected element
     * @throws std::runtime_error if the wheel is empty
     */
    E select() const {
        if (m_regions.empty()) {
            throw std::runtime_error("Cannot select from an empty RouletteWheel");
        }

        if (m_regions.size() == 1) {
            return m_regions[0].getElement();
        }

        const W totalWeight = calculateTotalWeight();
        const W randomValue = generateRandomWeight(totalWeight);

        return selectElementByWeight(randomValue);
    }

    /**
     * @brief Selects an element and returns it as an optional (safe version)
     * @return Optional containing the selected element, or nullopt if wheel is empty
     */
    std::optional<E> selectSafe() const {
        if (m_regions.empty()) {
            return std::nullopt;
        }
        return select();
    }

    /**
     * @brief Selects an element and modifies its weight
     * @param weightDelta Amount to add to the selected element's weight (can be negative)
     * @return The selected element
     * @throws std::runtime_error if the wheel is empty
     */
    E selectAndModifyWeight(W weightDelta = -1) {
        const E selectedElement = select();
        modifyElementWeight(selectedElement, weightDelta);
        return selectedElement;
    }

    /**
     * @brief Selects an element and removes it from the wheel
     * @return The selected element
     * @throws std::runtime_error if the wheel is empty
     */
    E selectAndRemove() {
        const E selectedElement = select();
        removeElement(selectedElement);
        return selectedElement;
    }

    /*** Modification Methods ***/

    /**
     * @brief Adds a new region to the wheel or combines weight if element exists
     * @param element The element to add
     * @param weight The weight for this element (must be positive)
     * @throws std::invalid_argument if weight is negative or zero
     */
    void addRegion(const E& element, W weight) {
        if (weight <= 0) {
            throw std::invalid_argument("Weight must be positive");
        }

        const auto existingIndex = findElementIndex(element);
        if (existingIndex.has_value()) {
            combineWeightAtIndex(*existingIndex, weight);
            return;
        }

        m_regions.emplace_back(element, weight);
    }

    /**
     * @brief Removes a specific element from the wheel
     * @param element The element to remove
     * @return true if element was found and removed, false otherwise
     */
    bool removeElement(const E& element) {
        const auto index = findElementIndex(element);
        if (!index.has_value()) {
            return false;
        }

        m_regions.erase(m_regions.begin() + *index);
        return true;
    }

    /**
     * @brief Removes all regions with weight <= 0
     * @return Number of regions removed
     */
    size_t removeInvalidRegions() {
        const size_t originalSize = m_regions.size();
        m_regions.erase(
            std::remove_if(m_regions.begin(), m_regions.end(),
                [](const WheelRegion<E, W>& region) {
                    return region.getWeight() <= 0;
                }),
            m_regions.end()
        );
        return originalSize - m_regions.size();
    }

    /*** Query Methods ***/

    /**
     * @brief Checks if the wheel has no regions
     * @return true if empty, false otherwise
     */
    bool empty() const {
        return m_regions.empty();
    }

    /**
     * @brief Gets the number of regions in the wheel
     * @return Number of regions
     */
    size_t size() const {
        return m_regions.size();
    }

    /**
     * @brief Calculates the selection probability for an element as a percentage
     * @param element The element to query
     * @return Percentage chance (0.0 to 100.0), or 0.0 if element not found
     */
    double getSelectionProbability(const E& element) const {
        if (m_regions.empty()) {
            return 0.0;
        }

        const auto elementWeight = findElementWeight(element);
        if (!elementWeight.has_value()) {
            return 0.0;
        }

        const W totalWeight = calculateTotalWeight();
        if (totalWeight <= 0) {
            return 0.0;
        }

        return (static_cast<double>(*elementWeight) / static_cast<double>(totalWeight)) * 100.0;
    }

    /**
     * @brief Gets a const reference to all wheel regions
     * @return Const reference to the regions vector
     */
    const std::vector<WheelRegion<E, W>>& getRegions() const {
        return m_regions;
    }

    /**
     * @brief Seeds the random number generator
     * @param seed The seed value
     */
    void seedRandom(unsigned int seed) {
        m_randomEngine.seed(seed);
    }

private:
    /*** Member Variables ***/
    std::vector<WheelRegion<E, W>> m_regions;
    mutable std::mt19937 m_randomEngine;

    /*** Private Helper Methods ***/

    /**
     * @brief Calculates the sum of all region weights
     * @return Total weight
     */
    W calculateTotalWeight() const {
        W totalWeight = 0;
        for (const auto& region : m_regions) {
            totalWeight += region.getWeight();
        }
        return totalWeight;
    }

    /**
     * @brief Generates a random weight value in the range [0, maxWeight)
     * @param maxWeight Upper bound (exclusive)
     * @return Random weight value
     */
    W generateRandomWeight(W maxWeight) const {
        if constexpr (std::is_integral_v<W>) {
            std::uniform_int_distribution<W> distribution(0, maxWeight - 1);
            return distribution(m_randomEngine);
        } else {
            std::uniform_real_distribution<W> distribution(0.0, static_cast<double>(maxWeight));
            return distribution(m_randomEngine);
        }
    }

    /**
     * @brief Selects an element based on a random weight value
     * @param randomValue The random value to use for selection
     * @return The selected element
     */
    E selectElementByWeight(W randomValue) const {
        W accumulatedWeight = 0;
        for (const auto& region : m_regions) {
            accumulatedWeight += region.getWeight();
            if (accumulatedWeight > randomValue) {
                return region.getElement();
            }
        }

        // Fallback to last element (handles floating-point rounding edge cases)
        return m_regions.back().getElement();
    }

    /**
     * @brief Finds the index of an element in the regions vector
     * @param element The element to find
     * @return Optional containing the index, or nullopt if not found
     */
    std::optional<size_t> findElementIndex(const E& element) const {
        for (size_t i = 0; i < m_regions.size(); ++i) {
            if (m_regions[i].getElement() == element) {
                return i;
            }
        }
        return std::nullopt;
    }

    /**
     * @brief Finds the weight of an element
     * @param element The element to find
     * @return Optional containing the weight, or nullopt if not found
     */
    std::optional<W> findElementWeight(const E& element) const {
        const auto index = findElementIndex(element);
        if (!index.has_value()) {
            return std::nullopt;
        }
        return m_regions[*index].getWeight();
    }

    /**
     * @brief Combines weight to an existing region at the specified index
     * @param index Index of the region
     * @param additionalWeight Weight to add
     */
    void combineWeightAtIndex(size_t index, W additionalWeight) {
        const W newWeight = m_regions[index].getWeight() + additionalWeight;
        m_regions[index].setWeight(newWeight);
    }

    /**
     * @brief Modifies the weight of an element and removes it if weight becomes invalid
     * @param element The element to modify
     * @param weightDelta Amount to add to the weight
     */
    void modifyElementWeight(const E& element, W weightDelta) {
        const auto index = findElementIndex(element);
        if (!index.has_value()) {
            return;
        }

        const W newWeight = m_regions[*index].getWeight() + weightDelta;
        if (newWeight <= 0) {
            m_regions.erase(m_regions.begin() + *index);
            return;
        }

        m_regions[*index].setWeight(newWeight);
    }

#ifdef USE_CEREAL
    friend class cereal::access;

    /**
     * @brief Serialization function for the cereal library
     *
     * Allows the RouletteWheel object to be serialized/deserialized.
     * Requires that both E and W types also have serialization support.
     * Note: Random engine state is not serialized.
     *
     * @see https://github.com/USCiLab/cereal
     */
    template <class Archive>
    void serialize(Archive& archive) {
        archive(m_regions);
    }
#endif
};


#endif // ROULETTE_WHEEL_HPP
