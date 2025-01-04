/**
 * @file Fr_WeightedDistribution.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 01-03-25
 *
 * @copyright Copyright (c) 2024
 *
 */
// 3193
#pragma once

#include <array>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <fstream>

#include "TextFileHandler.hpp"
#include "Fr_Random.hpp"

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

template<typename T, std::size_t N>
class WeightedDistribution
{

public:
 
    WeightedDistribution() {}

    WeightedDistribution(std::initializer_list<T> values, std::initializer_list<int> weights)
    {
        // Initializer list sizes do not match the template size of this class.
        if(values.size() != N || weights.size() != N)
        {
            TextFileHandler::add_to_buffer("[ERR] WeightedDistribution::WeightedDistribution(std::"
                "initializer_list<T> values, std::initializer_list<int> weights) -> "
                "Size of initializer list(s) do not match specified template size.\n");
            TextFileHandler::write("CrashLog.txt", Frost::APPEND);
            exit(1);
        }

        // No values to parse.
        if(m_values.size() == 0) return;

        // Temp index iterator.
        int i = 0;

        for(const T& _value : values)
        {
            m_values.at(i) = _value;
            ++i;
        }

        i = 0;

        for(const int num : weights)
        {
            m_weights.at(i) = num;
            ++i;
        }

        _sum_weights();
    }


    WeightedDistribution(const std::vector<T>& values, const std::vector<T>& weights)
    {
        // Initializer list sizes do not match the template size of this class.
        if(values.size() != N || weights.size() != N)
        {
            TextFileHandler::add_to_buffer("[ERR] WeightedDistribution::WeightedDistribution(std::"
                "const std::vector<T>& values, const std::vector<T>& weights) -> "
                "Size of vector(s) do not match specified template size.\n");
            TextFileHandler::write("CrashLog.txt", Frost::APPEND);
            exit(1);
        }

        // No values to parse.
        if(m_values.size() == 0) return;
        
        for(int i = 0; i < N; ++i)
        {
            m_values.at(i) = values.at(i);
            m_weights.at(i) = weights.at(i);
        }

        _sum_weights();
    }

    /** Returns the number of values that can be generated. */
    uint32_t size() const { return N; }

    /**
     * Selects and returns a value based on the weighted probability distribution.
     */
    const T& sample() const
    {
        const uint32_t RANDOM_VALUE = FrostRandom::get_random_int(1, m_cumulative_weights.back());

        // Find the the index in the cumulative weights array where its value is greater than or 
        // equal to the random generated number. 
        const uint32_t* it = 
            std::lower_bound(m_cumulative_weights.begin(), m_cumulative_weights.end(), RANDOM_VALUE);

        return m_values.at(it - m_cumulative_weights.begin());
    }

private:

    // Members

    std::array<T, N> m_values; // Available values that can be generated.
    
    /** Raw weights of the values. Each index of this array corresponds to the index of the 'm_values'
     * array. */
    std::array<uint32_t, N> m_weights;

    /**
     * @brief An array representing the cumulative sum of weights.
     *
     * This array is computed from the `m_weights` array to define ranges for random selection.
     * Each element in the array represents the cumulative weight up to that point.
     *
     * For example, if the `weights` array is {10, 20, 30}, the `cumulative_weights` array
     * will be {10, 30, 60}.
     *
     * This array is used to map a randomly generated number into
     * the appropriate range to select a value from the `m_values` array. The size of
     * each range is proportional to the weight of the corresponding value. */
    std::array<uint32_t, N> m_cumulative_weights; 


    // Methods

    /** Iterates through the 'm_weights' array to create the 'm_cumulative_weights' array using
     * partial sums. */
    void _sum_weights()
    {
        uint32_t sum {};

        // This method is called internally, so it is guaranteed that there is at least 1 value 
        // present. The first index of the cumulative weights array is always just the first index 
        // of the raw weights array, so that can be set here and then start computing partial sums 
        // from the next index, if it exists.

        m_cumulative_weights.at(0) = m_weights.at(0);

        // Iterate through each value of the weights, creating a partial sum from the previous 
        // weights.
        for(int i = 1; i < m_weights.size(); ++i)
        {
            // Partial sum can be created for this index by taking the partial sum of the last 
            // index and adding the raw weight of this index to it.
            m_cumulative_weights.at(i) = m_cumulative_weights.at(i - 1) + m_weights.at(i);
        }
    }
};
