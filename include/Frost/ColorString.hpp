/**
 * @file ColorString.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for ColorString.
 * @version 0.1
 * @date 2024-08-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>

/** Pure storage struct. A string with a color assigned to it. */
struct ColorString
{
    std::string content;
    std::string color;
};
