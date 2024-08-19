/**
 * @file UISimulationTools.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation of methods for simulating interactive interfaces for the user.
 * @version 0.1
 * @date 24-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "UISimulationTools.hpp"
#include "InputHandler.hpp"


void Frost::simulate_UI_selection(UISelectionContainer& data) 
{
    // Render ColorStrings before the cursor.
    for(int i = 0; i < data.cursor_index; ++i)
    {
        data.coh->add_str("   " + data.content.at(i).content + '\n', data.content.at(i).color);
    }

    // Render the ColorString at the cursor index.
    data.coh->add_str(" > ", *data.cursor_color);
    data.coh->add_str(data.content.at(data.cursor_index).content + '\n', 
        data.content.at(data.cursor_index).color);

    // Render ColorStrings after the cursor.
    for(int i = data.cursor_index + 1; i < data.content.size(); ++i)
    {
        data.coh->add_str("   " + data.content.at(i).content + '\n', data.content.at(i).color);
    } 

    // Pressed keys from the user that are available (not delayed).
    const std::vector<Key>& keys = InputHandler::get_pressed_and_available_keys();

    if(keys.size() == 0) return;

    // Handle only the first key that is registered.
    switch(*keys.begin())
    {
        case SDLK_RETURN:

            InputHandler::block_key_until_released(SDLK_RETURN);

            data.selected_index = data.cursor_index;
            return;

        case SDLK_w:    

            InputHandler::delay_key(SDLK_w);

            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                data.cursor_index = 0;
                return;
            }

            // Deduct 1 from the cursor index if it does not equal 0.
            data.cursor_index -= data.cursor_index != 0;
            return;

        case SDLK_s:    

            InputHandler::delay_key(SDLK_s);

            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                data.cursor_index = data.content.size() - 1;
                return;
            }

            // Add 1 to the cursor index if it is not at the end of the content.
            data.cursor_index += data.cursor_index != data.content.size() - 1;
            return;
    }
}

void Frost::simulate_UI_panel(UIPanelContainer& data) 
{
    // Rendering

    // Render UIItems before cursor index.
    for(int i = 0; i < data.cursor_index; ++i) 
    {
        data.content.at(i)->render_no_status();
        data.coh->add_new_line();
    }

    // The item at the cursor's position is selected.
    if(data.selected_index != -1) data.content.at(data.cursor_index)->render_selected();

    // The item is only hovered by the cursor.
    else data.content.at(data.cursor_index)->render_hovered();

    data.coh->add_new_line();

    // Render UIItems after cursor index.
    for(int i = data.cursor_index + 1; i < data.content.size(); ++i) 
    {
        data.content.at(i)->render_no_status();
        data.coh->add_new_line();
    }

    // Input Handling 

    // An item is selected
    if(data.selected_index > -1)
    {
        // Flag the selected item to handle input, and process the return status. If the return 
        // status is HOVERED, this item is no longer selected.
        if(data.content.at(data.selected_index)->handle_input() == UIItem::HOVERED)
            data.selected_index = -1;

        return;
    }

    // else : No item is selected.

    // Get pressed keys from the user that are available (not delayed).
    const std::vector<Key>& keys = InputHandler::get_pressed_and_available_keys();

    if(keys.size() == 0) return;

    // Handle only the first key that is registered.
    switch(*keys.begin())
    {
        case SDLK_RETURN:   

            // Select the item at the cursor's index.

            InputHandler::block_key_until_released(SDLK_RETURN);

            data.selected_index = data.cursor_index;
            return;

        case SDLK_w:    

            InputHandler::delay_key(SDLK_w);

            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                data.cursor_index = 0;
                return;
            }

            // Deduct 1 from the cursor index if it does not equal 0.
            data.cursor_index -= data.cursor_index != 0;
            return;

        case SDLK_s:    

            InputHandler::delay_key(SDLK_s);

            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                data.cursor_index = data.content.size() - 1;
                return;
            }

            // Add 1 to the cursor index if it is not at the end of the content.
            data.cursor_index += data.cursor_index != data.content.size() - 1; 
    }
}