#pragma once

#include <cmath>
#include <cstdint>


namespace Frost
{
    /** Returns the passed uint8_t clamped to the maximum if it exceeds it. 
     * 
     * @param num Num to clamp.
     * @param max Maximum value.
     */
    uint8_t clamp_uint8_to_maximum(uint8_t num, uint8_t max);

    /** Returns the passed uint16_t clamped to the minimum if it is below it. 
     * 
     * @param num Num to clamp.
     * @param min Minimum value.
    */
    uint16_t clamp_uint16_to_minimum(uint16_t num, uint16_t min);

    /** Returns the passed uint16_t clamped to the maximum if it exceeds it. 
     * 
     * @param num Num to clamp.
     * @param max Maximum value.
     */
    uint16_t clamp_uint16_to_maximum(uint16_t num, uint16_t max);

    /** Returns the larger of the two passed uint16_ts 
     * 
     * @param first First number
     * @param second Second number
     */
    uint16_t return_largest_of_uint16s(uint16_t first, uint16_t second);

    /** Calculates the ratio of num to its min and max, and applies this ratio to the 
     * passed range to return the portioned value.
     * 
     * @param num Value to calculate the ratio from the min and max.
     * @param min Minimum value of the num.
     * @param max Maximum value of the num.
     * @param range Value that the ratio will portion.
     */
    int get_ratio_of_range(int num, int min, int max, int range);

    /** Returns the passed float clamped to the minimum if it is below it. 
     * 
     * @param num Num to clamp.
     * @param min Minimum value.
    */
    float clamp_float_to_minimum(float num, float min);

    /** Returns the num clamped to the maximum if it is above it.
     * 
     * @param num Num to clamp.
     * @param max Maximum value.
     */
    float clamp_float_to_maximum(float num, float max);
};
