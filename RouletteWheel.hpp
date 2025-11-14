#include "classes/WheelRegion.hpp"


/**
 * @brief An object used to store data on a theoretical roulette wheel.
 *        Mainly used for running the weighted random selection algorithm, the "roulette wheel selection algorithm".
 */
template<typename E, typename W>
class RouletteWheel
{
public:
    /*** Variables ***/
    std::vector< WheelRegion<E,W> > wheelRegions; //A linear data structure storing all of the regions of the wheel

    /*** Constructors ***/
    //Default
    RouletteWheel()
    {
    }


    /**
     * @brief Converts an unordered map to a roulette wheel object
     */
    RouletteWheel(  const std::unordered_map<E, W> unorderedMap   )
    {
        //For each key in the unordered map
        for(const auto & [key, value] : unorderedMap)
        {
            //Add a region to the wheel, storing the key of the map as the element of the region and the value as the weight of the region
            addWheelRegion(key, value);
        }
    }


    /**
     * @brief Converts a vector of tuples into a roulette wheel object
     */
    RouletteWheel(  std::vector< std::tuple<E,W> > vecOfTuples )
    {
        //For each element in the vector of tuples
        for(const auto & element : vecOfTuples)
        {
            //Add a region to the wheel, storing the first element in the tuple as the element of the region and the second as the weight of the region
            addWheelRegion(std::get<0>(element), std::get<1>(element));
        }
    }



    /***** METHODS *****/
    
    /**
     * @brief Run the roulette wheel selection algorithm. 
     */
    E select() const
    {
        //If we try to select from an empty wheel, just return a static, default object of type E
        if( wheelRegions.empty() )
        {
            static E dummyElement;
            return dummyElement;
        }
        //If we only have one region on the wheel, return the element of the single region
        else if( wheelRegions.size() == 1 )
        {
            return wheelRegions.at(0).getElement();
        }

        //Get the sum of the weights for all entries in wheelRegions
        W sumOfWeights = getSumOfWeights();

		//Now generate a random number between 0 and the sumOfWeights
		W randomNumber = static_cast <W> (std::rand() / ( static_cast <W> (RAND_MAX/(sumOfWeights))));

		//Now sum up our weights until our partial sum is greater than the randomly chosen number
		float partialSum = 0;
		for(int i = 0; i < wheelRegions.size(); i++)
		{
			partialSum += wheelRegions.at(i).getWeight();
			if( partialSum >= randomNumber )
			{
                //The partialSum is greater than the randomNumber. This is where the wheel stops spinning!
                WheelRegion selectedWheelRegion = wheelRegions.at(i);
				return selectedWheelRegion.getElement();
			}	
		}

		std::cout << "RouletteWheel::select() error, couldn't return an element. Partial sum reached " << partialSum <<
                     " and the randomNumber generated was " << randomNumber << std::endl;
		getch();
		E defaultItemToReturn;
		return defaultItemToReturn;
    }


    /**
     * @brief Selects an element from the Roulette wheel and then changes the weight of the selected element on
     *        the wheel before returning it. If the weight becomes less than or equal to zero, the wheel
     *        region is removed.
     */
    E selectAndChangeWeight( W weightChange = -1 )
    {
        // First select an element using the standard selection algorithm
        E selectedElement = select();

        // Find the index of the selected element
        int index = getIndexOfElement(selectedElement);

        // If found, change the weight
        if(index != -1)
        {
            wheelRegions.at(index).weight += weightChange;

            // Remove the region if weight is now 0 or less
            if(wheelRegions.at(index).getWeight() <= 0)
            {
                wheelRegions.erase(wheelRegions.begin() + index);
            }
        }

        return selectedElement;
    }


    /**
     * @brief Runs the roulette wheel selection algorithm, returning the selected wheel region element and also removing it from the wheel.
     */
    E selectAndRemove()
    {
        // First select an element using the standard selection algorithm
        E selectedElement = select();

        // Find the index of the selected element
        int index = getIndexOfElement(selectedElement);

        // If found, remove it from the wheel
        if(index != -1)
        {
            wheelRegions.erase(wheelRegions.begin() + index);
        }

        return selectedElement;
    }


    /**
     * @brief Adds a region to the wheel. If an element already exists, combines the weights.
     */
    void addWheelRegion(    const E regionElement,
                            const W regionWeight    )
    {
        // Check if this element already exists in the wheel
        for (auto& existingRegion : wheelRegions) {
            if (existingRegion.getElement() == regionElement) {
                // Element found - combine the weights
                W combinedWeight = existingRegion.getWeight() + regionWeight;
                existingRegion.weight = combinedWeight;
                return;
            }
        }
        
        // Element not found - add as new region
        WheelRegion wheelRegion = WheelRegion(regionElement, regionWeight);
        wheelRegions.push_back(wheelRegion);
    }


    /**
	 * @brief Modify wheelRegions, such that all wheelRegions containing a weight equal to zero are removed.
	 */
	void removeInvalidRegions()
	{
		wheelRegions.erase( std::remove_if(wheelRegions.begin(), wheelRegions.end(),
                            [](WheelRegion<E,W> wheelRegion) { return wheelRegion.getWeight() <= 0; }), wheelRegions.end());
	}


    /**
     * @brief Returns true if the roulette wheel has no regions
     */
    bool empty() const
    {
        return wheelRegions.empty();
    }

    /**
     * @brief Returns the percentage chance (0.0 to 100.0) that a given element will be selected
     * @param element The element to calculate the selection percentage for
     * @return Percentage chance (0.0 to 100.0), or 0.0 if element not found
     */
    float getSelectChance(const E& element) const
    {
        if (wheelRegions.empty()) {
            return 0.0f;
        }

        // Find the wheel region containing this element
        W elementWeight = 0;
        bool elementFound = false;
        
        for (const auto& wheelRegion : wheelRegions) {
            if (wheelRegion.getElement() == element) {
                elementWeight = wheelRegion.getWeight();
                elementFound = true;
                break;
            }
        }
        
        if (!elementFound) {
            return 0.0f;
        }
        
        // Calculate total weight
        W totalWeight = getSumOfWeights();
        
        if (totalWeight <= 0) {
            return 0.0f;
        }
        
        // Return percentage (element weight / total weight * 100)
        return static_cast<float>(elementWeight) / static_cast<float>(totalWeight) * 100.0f;
    }


private:


    /*** Methods (Private) ***/

    /**
     * @brief Calculates the sum of all of each wheelRegion's weight and returns it.
     */
    W getSumOfWeights() const
    {
        W weightSum = 0;
		//For each region of the wheel
		for(int i = 0; i < wheelRegions.size(); i++)
		{
			weightSum += wheelRegions.at(i).getWeight();
		}
		return weightSum;
    }


    /**
     * @brief Finds the index of a wheel region containing the given element.
     * @param element The element to search for
     * @return The index of the wheel region, or -1 if not found
     */
    int getIndexOfElement(const E& element) const
    {
        for(int i = 0; i < wheelRegions.size(); i++)
        {
            if(wheelRegions.at(i).getElement() == element)
            {
                return i;
            }
        }
        return -1;
    }

    
#ifdef USE_CEREAL
    // Make cereal::access a friend only if cereal is enabled
    friend class cereal::access;
    /**
     * @brief The function that allows the cereal library to serialize the RouletteWheel object.
     *        In order to use this function, the E and W types must also have serialization functions
     *        defined for them as well.
     *
     *        Learn more about the cereal library here: https://github.com/USCiLab/cereal
     */
    template <class Archive>
    void serialize(Archive & archive)
    {
        archive( wheelRegions );
    }
#endif
};