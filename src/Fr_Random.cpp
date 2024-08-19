/**
 * @file Fr_Random.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implemenation for FrostRandom.
 * @version 0.1
 * @date 2024-08-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Fr_Random.hpp"
#include <random>


// Static Members

std::mt19937 FrostRandom::generator =  std::mt19937(std::random_device{}() );


// Public

void FrostRandom::seed(uint64_t seed) { generator = std::mt19937(seed); }

int FrostRandom::get_random_int(int lower, int higher)
{
    return std::uniform_int_distribution<int>(lower, higher)(generator);
}

