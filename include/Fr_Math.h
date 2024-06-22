#pragma once

#include <cmath>
#include <cstdint>


namespace Frost
{
    /** Returns the num clamped to the minimum if it is below it. 
     * 
     * @param num Num to clamp.
     * @param min Minimum value.
    */
    uint16_t clamp_uint16_to_minimum(uint16_t num, uint16_t min);

    /** Returns the num clamped to the minimum if it is below it. 
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
