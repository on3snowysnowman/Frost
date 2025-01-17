/**
 * @file Fr_Math.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration & implementation for various functions handling integer clamping,
 * returning larger of two numbers and obtaining a ratio from a range.
 * @version 0.2
 * @date 2024-08-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <cmath>
#include <cstdint>

namespace Frost
{
    /** Returns if the passed num is even.
     * @tparam T Type of the num.
     * @param num Num to check.
     */
    template<typename T>
    bool is_even(T num) { return num & 1; }

    /**
     * Returns the passed num clamped to the minimum if it exceeds it.
     * 
     * @tparam T Type of the num.
     * @param num Num to clamp.
     * @param min Minimjm value.
     */
    template<typename T> 
    T clamp_num_to_minimum(T num, T min) { return num < min ? min : num; }

    /**
     * Returns the passed num clamped to the maximum if it exceeds it.
     * 
     * @tparam T Type of the num.
     * @param num Num to clamp.
     * @param max Maximum value.
     */
    template<typename T> 
    T clamp_num_to_maximum(T num, T max) { return num > max ? max : num; }

    /**
     * @brief Returns the larger of two nums.
     * 
     * @tparam T Type of the num.
     */
    template<typename T>
    T return_largest_of_two_nums(T first, T second) { return first > second ? first : second; }

    /** Calculates the ratio of num to its min and max, and applies this ratio to the 
     * passed range to return the portioned value.
     * 
     * @param num Value to calculate the ratio from the min and max.
     * @param min Minimum value of the num.
     * @param max Maximum value of the num.
     * @param range Value that the ratio will portion.
     */
    int get_ratio_of_range(int num, int min, int max, int range);
};
