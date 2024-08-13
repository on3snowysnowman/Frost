#include <cmath>

#include "UIDisplayTools.hpp"
#include "Fr_Math.hpp"


// Public

void Frost::display_colored_int(ConsoleOutputHandler& coh, int num, int min, int max,
    const std::vector<std::string> color_distribution)
{
    if(min >= num)
    {
        coh.add_str(std::to_string(num), color_distribution.at(0));
        return;
    };

    if(max <= num)
    {
        coh.add_str(std::to_string(num), *(--color_distribution.end()));
        return;
    }
    coh.add_str(std::to_string(num), color_distribution.at(
        Frost::get_ratio_of_range(num, min, max, color_distribution.size())));
}

void Frost::display_single_color_meter(ConsoleOutputHandler& coh, int num, int min, 
    int max, const std::string color)
{
    coh.add_str(std::to_string(num) + " [");

    for(int i = 0; i < num; ++i) coh.add_ch('/', color);
    
    for(int i = num; i < 10; ++i) coh.add_ch('/');

    coh.add_str("] " + std::to_string(max));
}

void Frost::display_multi_color_meter(ConsoleOutputHandler& coh, int num, int min, 
    int max, std::vector<std::string> color_distribution)
{
    const std::string& TARG_COLOR = color_distribution.at(
    Frost::get_ratio_of_range(num, min, max, color_distribution.size()));

    coh.add_str(std::to_string(num) + " [");

    for(int i = 0; i < num; ++i) coh.add_ch('/', TARG_COLOR);
    
    for(int i = num; i < 10; ++i) coh.add_ch('/');

    coh.add_str("] " + std::to_string(max));
}

