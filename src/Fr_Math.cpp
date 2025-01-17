#include "Fr_Math.hpp"

int Frost::get_ratio_of_range(int num, int min, int max, int range)
{
    float ratio = float((num - min) / 
        ((max - min) * 1.0));

    return range * ratio;
}


