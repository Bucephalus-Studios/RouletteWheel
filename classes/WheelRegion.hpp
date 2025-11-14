#ifndef WHEEL_REGION_HPP
#define WHEEL_REGION_HPP

#include <utility>  // For std::move

/**
 * @brief A region within a RouletteWheel containing an element and its selection weight.
 *
 * This class represents a single segment of the roulette wheel, storing an element
 * and its associated probability weight for the roulette wheel selection algorithm.
 *
 * @tparam E Element type to store
 * @tparam W Weight type (typically numeric: int, float, double)
 */
template<typename E, typename W>
class WheelRegion {
public:
    /**
     * @brief Default constructor - creates a region with default-initialized values
     */
    WheelRegion() = default;

    /**
     * @brief Constructs a wheel region with the specified element and weight
     * @param element The element to store in this region
     * @param weight The probability weight for selecting this element
     */
    WheelRegion(E element, W weight)
        : m_element(std::move(element))
        , m_weight(weight) {
    }

    /**
     * @brief Gets the element stored in this wheel region
     * @return Const reference to the element
     */
    const E& getElement() const {
        return m_element;
    }

    /**
     * @brief Gets the weight of this wheel region
     * @return The weight value
     */
    W getWeight() const {
        return m_weight;
    }

    /**
     * @brief Sets the weight of this wheel region
     * @param weight The new weight value
     */
    void setWeight(W weight) {
        m_weight = weight;
    }

private:
    E m_element{}; ///< The element stored in this wheel region
    W m_weight{};  ///< The selection weight for this region

#ifdef USE_CEREAL
    friend class cereal::access;

    /**
     * @brief Serialization function for the cereal library
     *
     * Allows the WheelRegion object to be serialized/deserialized.
     * Requires that both E and W types also have serialization support.
     *
     * @see https://github.com/USCiLab/cereal
     */
    template <class Archive>
    void serialize(Archive& archive) {
        archive(m_element, m_weight);
    }
#endif
};

#endif // WHEEL_REGION_HPP
