/**
 * @brief An object stored within a RouletteWheel's wheelRegions vector. Contains the elements
 *        stored in the RouletteWheel as wheel as their associated weight for being selected 
 *        with the roulette wheel selection algorithm.
 */
template<typename E, typename W> 
class WheelRegion
{
public:
    /*** Variables ***/
    E element; //The element stored at this region of the wheel
    W weight; //The weight of this wheel region to be selected by the roulette wheel selection algorithm

    /*** Constructors ***/

    //default
    WheelRegion()
    {
    }

    //parametric
    WheelRegion(    const E elementParam,
                    const W weightParam   )
    {
        element = elementParam;
        weight = weightParam;
    }


    /***** METHODS *****/
    E getElement() const
    {
        return element;
    }


    W getWeight() const
    {
        return weight;
    }
    


private:

#ifdef USE_CEREAL
    // Make cereal::access a friend only if cereal is enabled
    friend class cereal::access;
    /**
     * @brief The function that allows the cereal library to serialize the WheelRegion object.
     *        In order to use this function, the E and W types must also have serialization functions
     *        defined for them as well.
     *
     *        Learn more about the cereal library here: https://github.com/USCiLab/cereal
     */
    template <class Archive>
    void serialize(Archive & archive)
    {
        archive( element, weight );
    }
#endif
};