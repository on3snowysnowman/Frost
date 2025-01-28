/**
 * @file Fr_WeightedDistribution.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class declaration & implementation
 * @version 0.2
 * @date 01-03-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <array>
#include <cstdint>
#include <initializer_list>
#include <algorithm>
#include <iostream>

#include "TextFileHandler.hpp"
#include"Fr_Random.hpp"

/**
 * @class WeightedDistribution
 * @brief A class for selecting random values based on weighted probabilities.
 *
 * This class provides functionality to perform weighted random selection
 * from a fixed-size collection of values and their associated weights.
 * The probability of selecting a value is proportional to its weight.
 *
 * @tparam N The fixed size of the arrays for values and weights.
 * @tparam T The type of the values being distributed.
 */
template<typename T,  uint64_t N>
class WeightedDistribution
{

public:

    WeightedDistribution() {}

    WeightedDistribution(std::initializer_list<T> values, std::initializer_list<uint32_t> weights)
    {
        set_values(values);
        set_weights(weights);
    }

    WeightedDistribution(const std::array<T, N>& values, const std::array<uint32_t, N>& weights)
    {
        set_values(values);
        set_values(weights);
    }

    WeightedDistribution(const std::vector<T>& values, const std::vector<uint32_t>& weights)
    {
        set_values(values);
        set_values(weights);
    }

    /** Sets the value at the passed index. */
    void set_value(uint32_t index, T value)
    {
        _handle_index_check(index);

        m_values.at(index) = value;
    }

    /** Sets the weight at the passed index. Weights are resummed after modification.  */
    void set_weight(uint32_t index, uint32_t weight)
    {
        _handle_index_check(index);

        m_weights.at(index) = weight;

        _sum_weights();        
    }

    /** Sets the sampleable values to a new set of values using 'values' as a iterable container
     * to fetch the values from. The 'values' paramater must have a begin() and end() function to
     * be parsed like an array for the new values. */
    template<typename Container_T>
    void set_values(const Container_T& values)
    {
        if(values.size() != N)
        {
            TextFileHandler::add_to_buffer("WeightedDistribution::set_values(const Container_T&"
                " values) where 'Container_T' = " + std::string(typeid(Container_T).name()) + 
                "-> Passed container size does not match specified template size.\n");
            TextFileHandler::write("CrashLog.txt", Frost::APPEND);
            exit(1);
        }

        typename Container_T::const_iterator it = values.begin();

        // Get the number of elements in the passed arbitrary container.
        const uint32_t NUM_VALUES = std::distance(it, values.end());

        for(uint32_t i = 0; i < NUM_VALUES; ++i)
        {
            m_values.at(i) = *it;
            ++it;
        }
    }

    /** Sets the weights of the sampleable values to a new set of weights using 'weights' as a 
     * iterable container to fetch the values from. The 'weights' paramater must have a begin() 
     * and end() function to be parsed like an array for the new weights. */
    template<typename Container_T>
    void set_weights(const Container_T& weights)
    {
        if(weights.size() != N)
        {
            TextFileHandler::add_to_buffer("WeightedDistribution::set_weights(const Container_T&"
                " weights) where 'Container_T' = " + std::string(typeid(Container_T).name()) + 
                "-> Passed container size does not match specified template size.\n");
            TextFileHandler::write("CrashLog.txt", Frost::APPEND);
            exit(1);
        }

        typename Container_T::const_iterator it = weights.begin();

        // // Get the number of elements in the passed arbitrary container.
        const uint32_t NUM_VALUES = std::distance(it, weights.end());

        // Stores the total sum of the weights as they are added.
        uint32_t sum_weights {};

        for(uint32_t i = 0; i < NUM_VALUES; ++i)        
        {
            sum_weights += static_cast<uint32_t>(*it);

            m_weights.at(i) = static_cast<uint32_t>(*it);
            m_cumulative_weights.at(i) = sum_weights;

            ++it;
        }
    }

    /** Samples and returns a weighted random value from the specified values. */
    const T& sample() const
    {
        const uint32_t RAND_NUM = 
            FrostRandom::get_random_num<uint32_t>(1, m_cumulative_weights.back());

        const uint32_t* it = std::lower_bound(m_cumulative_weights.begin(), m_cumulative_weights.end(),
            RAND_NUM);

        return m_values.at(std::distance(m_cumulative_weights.begin(), it));
    }

    const std::array<T, N>& get_values() const { return m_values; }

    const std::array<uint32_t, N>& get_weights() const { return m_weights; }

private:

    // Members

    // Values that will be sample from.
    std::array<T, N> m_values;

    // Weights of each of the sampleable values.
    std::array<uint32_t, N> m_weights;

    // Cumulative sum of the 'm_weights' array.
    std::array<uint32_t, N> m_cumulative_weights;


    // Methods

    void _sum_weights()
    {
        uint32_t sum_weights {};

        for(uint32_t i = 0; i < N; ++i)
        {   
            sum_weights += m_weights.at(i);
            m_cumulative_weights.at(i) = sum_weights;
        }
    }

    /** Checks if the passed index is out of bounds of the internal arrays. If it is, an error will
     * be outputted and the program will safely crash. */
    void _handle_index_check(uint32_t index)
    {
        // Index is valid.
        if(index < N) return;
    
        TextFileHandler::add_to_buffer("[ERR WeightedDistribution::_handle_index_check(uint32_t "
            "index) where 'index' = " + std::to_string(index) + " -> Requested index out of "
            "range.");
        TextFileHandler::write("CrashLog.txt", Frost::APPEND);
        exit(1);
    }   
};
