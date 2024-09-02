/**
 * @file Fr_Random.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for FrostRandom.
 * @version 0.1
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
    static void seed(uint64_t seed);

    /** Returns a psuedo random number between the ranges of lower and higher.
     * 
     * @param lower Lower bound of random range.
     * @param higher Higher bound of random range. */
    static int get_random_int(int lower, int higher);

private:

    static std::mt19937 generator;
};
 