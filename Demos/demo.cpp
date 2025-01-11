#define SDL_MAIN_HANDLED

#include "Fr_WeightedDistribution.hpp"

int main()
{
    WeightedDistribution<int, 3> wd({1, 2, 3}, {3, 2, 1});


    return 0;
}
