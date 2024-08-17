/**
 * @file UISimulationContainers.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Contains pure storage structs that contain data for UISimulation.
 * @version 0.1
 * @date 2024-08-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once


#include <array>
#include <initializer_list>

#include "ColorString.hpp"


/** Pure storage struct. Stores data that UISimulationTools::UISimulateSelelection processes. */
template<std::size_t N>
struct UISelectionContainer
{

    UISelectionContainer() {}

    UISelectionContainer(std::initializer_list<ColorString> _content)
    {
        for(int i = 0; i < _content.size(); ++i)
        {
            content.at(i) = std::move(_content.at(i));
        }
    }

    // If an item has been selected during simulation.
    bool item_is_selected = false;

    // The choices that can be selected during simulation.
    std::array<ColorString, N> content;

    // Position of the cursor inside content.
    int16_t cursor_position {};
};

/** Pure storage struct. Stoes data that UISimulationTools::UISimulateMenu processes. */
struct UISimulationContainer
{

};
