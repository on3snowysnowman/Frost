/**
 * @file Fr_Random.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for FrostRandom.
 * @version 0.2
 * @date 2024-08-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

 #pragma once

 
 #include <random>


/** Static class that provides useful functions for generating psuedo random numbers. Internal 
 * generator can be seeded using the $seed method for reproducing output. */
class FrostRandom
{

public:

    /** Seeds the generator.
     *
     * @param seed Target seed. */
    static void seed(uint64_t seed)
    {
        generator = std::mt19937(seed);
    }

    /** Returns a psuedo random number between the ranges of lower and higher.
     * 
     * If 'lower' is greater than 'higher', higher will be returned.
     * 
     * @param lower Lower bound of random range.
     * @param higher Higher bound of random range. */
    template<typename T>
    static T get_random_num(T lower, T higher)
    {
        if(lower >= higher) return higher;

        return std::uniform_int_distribution<T>(lower, higher)(generator);
    }

private:

    static inline std::mt19937 generator = std::mt19937(std::random_device{}());
};
 