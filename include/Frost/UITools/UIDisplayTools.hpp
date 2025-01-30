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

#include <vector>

#include "ConsoleOutputHandler.hpp"
#include "ColorString.hpp"
#include "Fr_Math.hpp"

namespace Frost
{
    /**
     * @brief Generates a ColorString containing `num` with a color applied to it based on
     * the ratio of`num` from the range `min` and `max`. 
     * 
     * The `color_distribution` vector is used to specify an arbitrary list of color options that 
     * can be selected from, depending on the value of `num`. From the ratio of `num` to its 
     * extremes, an index will be selected from this vector using said ratio. For instance, if `num`
     * is very close to `min`, the first index of `color_distribution` will be selected: "Red" in the 
     * default's case. This is meant to represent that the color red is bad, or danger, and as the
     * value of `num` increases, it will parse the available colors, eventually reaching "Green" to
     * represent good, or safety as it closes in on the `max` value.
     * 
     * `num`is not required to be within the range of `min` and `max`, and there will be no error 
     * if this is the case. If `num` breaches these extremes, the first or last color will be
     * selected respective to the breached extreme.
     * 
     * @attention There is no bounds check for `color_distribution`. Ensure a non empty vector is 
     * passed or an std::out_of_range error will be thrown. 
     * 
     * @tparam T Type of the num. 
     * @param num Value of the num.
     * @param min Minimum value of 'num'.
     * @param max Maximum value of `num`.
     * @param color_distribution Colors that `num` may be applied with. 
     */
    template<typename T>
    ColorString generate_colored_int(T num, T min, T max, 
        const std::vector<std::string>& color_distribution = {"Red", "Orange", "Yellow", "Green"})
    {
        if(num <= min)
        {
            // Not using front() here since there is no bounds check for the vector up to this 
            // point. It is intended that the program crashes if the user passes an empty vector.
            return ColorString {std::to_string(num), color_distribution.at(0)};
        }

        else if(num >= max)
        {
            // Same reason for not using back() here as front().
            return ColorString {std::to_string(num), color_distribution.at(color_distribution.size() - 1)};
        }

        // Calculate the ratio of the num between its min and max for the color, then apply that 
        // ratio to the size of the vector to get the color. 
        ColorString {std::to_string(num), color_distribution.at(
            Frost::get_ratio_of_range(num, min, max, color_distribution.size()))};
    }

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
    void display_single_color_meter(ConsoleOutputHandler& coh, int num, int min, int max,
        const std::string color);

    /** Displays a horizontal "meter" on screen, which colors a portion of the meter depending
     * on the ratio of the number to its minimum and maximum. This ratio is also used to select
     * which color to pick in the color_distribution vector.
     * 
     * @param coh ConsoleOutputHandler to use for output.
     * @param num Value to calculate the ratio from the min and max.
     * @param min Minimum value of the num.
     * @param max Maximum value of the num.
     * @param color_distribution Colors that will be selected from using the ratio.
     */
    void display_multi_color_meter(ConsoleOutputHandler& coh, int num, int min, int max,
        const std::vector<std::string> color_distribution = {"Red", "Orange", "Yellow", "Green"});
};
