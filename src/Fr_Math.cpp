#include "Fr_Math.hpp"


uint8_t Frost::clamp_uint8_to_maximum(uint8_t num, uint8_t max)
{ return num > max ? max : num; }

uint16_t Frost::clamp_uint16_to_minimum(uint16_t num, uint16_t min)
{ return num < min ? min : num; }

uint16_t Frost::clamp_uint16_to_maximum(uint16_t num, uint16_t max)
{ return num > max ? max : num; }

uint16_t Frost::return_largest_of_uint16s(uint16_t first, uint16_t second)
{ return first > second ? first : second; }

float Frost::clamp_float_to_minimum(float num, float min)
{ return num < min ? min : num; }

float Frost::clamp_float_to_maximum(float num, float max)
{ return num > max ? max : num; }
