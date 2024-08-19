/**
 * @file UISimulationTools.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration of methods for simulating interactive interfaces for the user.
 * @version 0.1
 * @date 24-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "UISimulationContainers.hpp"

namespace Frost
{

    /** Provides a user interface simulation for selecting a ColorString choice out of a selection
     * of choices. Handles user input so users can navigate the selection and select a chioce. 
     * Read Documentation/UITools.txt for more info.
     *
     * @param data Data for simulation. */
    void simulate_UI_selection(UISelectionContainer& data);

    /** Provides a user interface simulation for interacting with a "panel" of modular UIItems. 
     * Handles user input so users can navigate the UIItems and interface with them. Read
     * Documentation/UITools.txt for more info.
     *
     *@param data Data for simulation. */
    void simulate_UI_panel(UIPanelContainer& data);
};
