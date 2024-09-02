/**
 * @file UIDisplayTools.hpp
 * @author Joel Height (on3snowysnowman@gmail.com)
 * @brief Collection of convenient methods for rendering basic UI Elements on screen.
 * @version 0.1
 * @date 2024-08-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <cstdint>
#include <vector>

#include "ConsoleOutputHandler.hpp"

namespace Frost
{

    /** Displays the passed num to the screen, with a color applied to it. This color is 
     * calculated using the ratio of the number in comparison to its minimum and maximum, then
     * this ratio is used to pick the color in the color_distribution vector.
     * 
     * @param coh ConsoleOutputHandler to use for output.
     * @param num Target number to render.
     * @param min Minimum value of the number.
     * @param max Maximium value of the number.
     * @param color_distribution Colors that will be selected from using the ratio.
     */
    static void display_colored_int(ConsoleOutputHandler& coh, int num, int min, int max,
        const std::vector<std::string> color_distribution = {"Red", "Orange", "Yellow", "Green"});

    /** Displays a horizontal "meter" on screen, which colors a portion of the meter depending
     * on the ratio of the number to its minimum and maximum. The color used is passed as a 
     * parameter.
     * 
     * @param coh ConsoleOutputHandler to use for output.
     * @param num Value to calculate the ratio from the min and max.
     * @param min Minimum value of the num.
     * @param max Maximum value of the num.
     * @param color Color to apply to the meter.
    */
    static void display_single_color_meter(ConsoleOutputHandler& coh, int num, int min, int max,
        const std::string color);

    /** Displays a horizontal "meter" on screen, which colors a portion of the meter depending
     * on the ratio of the number to its minimim and maximum. This ratio is also used to select
     * which color to pick in the color_distribution vector.
     * 
     * @param coh ConsoleOutputHandler to use for output.
     * @param num Value to calculate the ratio from the min and max.
     * @param min Minimum value of the num.
     * @param max Maximum value of the num.
     * @param color_distribution Colors that will be selected from using the ratio.
     */
    static void display_multi_color_meter(ConsoleOutputHandler& coh, int num, int min, int max,
        const std::vector<std::string> color_distribution = {"Red", "Orange", "Yellow", "Green"});
};
