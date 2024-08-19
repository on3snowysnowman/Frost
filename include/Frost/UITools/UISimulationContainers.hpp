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


#include <vector>
#include <initializer_list>
#include <string>

#include "ColorString.hpp"
#include "UIItem.hpp"
#include "ConsoleOutputHandler.hpp"


/**
 * Stores data that UISimulationTools::simulate_UI_selection uses to simulate
 * a selection interface. */
class UISelectionContainer
{

public:

    UISelectionContainer() {}

    UISelectionContainer(ConsoleOutputHandler& _coh, std::initializer_list<ColorString> _content)
    {
        coh = &_coh;

        for(const ColorString& str : _content)
        {
            content.push_back(std::move(str));
        }
    }

    /** Resets the selected_index and cursor_index to their default position.  */
    void reset() 
    {
        cursor_index = 0;
        selected_index = -1;
    }

    // Index of the cursor inside content.
    int16_t cursor_index {};

    /**
     * Index of the selected item inside content. If this value is -1, this specifies there is no 
     * item selected. */
    int16_t selected_index = -1;

    // Pointer to the string that determines the cursor color.
    std::string* cursor_color; 

    // The choices that can be selected during simulation.
    std::vector<ColorString> content;

    ConsoleOutputHandler* coh;
};

/** Stores data that UISimulationTools::simulate_UI_panel uses to simulate a 
 * menu interface. */
class UIPanelContainer
{

public:

    UIPanelContainer() {}

    UIPanelContainer(ConsoleOutputHandler& _coh)
    {
        coh = &_coh;
    }

    /** Resets the selected_index and cursor_index to their default position.  */
    void reset() 
    {
        cursor_index = 0;
        selected_index = -1;
    }


    // Index of the cursor inside content.
    int16_t cursor_index {};

    /**
     * Index of the selected item inside content. If this value is -1, this specifies there is no 
     * item selected. */
    int16_t selected_index = -1;

    // Pointer to the string that determines the cursor color.
    std::string* cursor_color; 

    // The UIItems that are used to simulate the menu in UISimulationTools::UISimulateMenu.
    std::vector<UIItem*> content;

    ConsoleOutputHandler* coh;
};
