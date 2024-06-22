#include "Fr_Math.h"


uint16_t Frost::clamp_uint16_to_minimum(uint16_t num, uint16_t min)
{ return num < min ? min : num; }

float Frost::clamp_float_to_minimum(float num, float min)
{ return num < min ? min : num; }

float Frost::clamp_float_to_maximum(float num, float max)
{ return num > max ? max : num; }
