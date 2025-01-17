#include <iostream>
#include <map>

#include "Fr_WeightedDistribution.hpp"

int main()
{
    WeightedDistribution<int, 4> wd ({1, 2, 3, 4}, {1, 1, 1, 1});

    std::map<int, int> results;
 
    const int TRIALS = 100000;

    for(int i = 0; i < TRIALS; ++i)
    {
        ++results[wd.sample()];
    }

    for(auto _pair : results)
    {
        std::cout << _pair.first << ": " << (_pair.second / (1.0 * TRIALS)) * 100 << "%\n";
    }

    return 0;
}
