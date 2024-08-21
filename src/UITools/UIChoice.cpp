/**
 * @file UIChoice.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation for UIChoice.
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "UIChoice.hpp"
#include "Fr_Math.hpp"
#include "InputHandler.hpp"


// Constructors / Deconstructor

UIChoice::UIChoice(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name,
    std::initializer_list<ColorString> content, uint16_t initial_cursor_index, 
    uint16_t initial_selected_index) : UIItem(coh, cursor_color, "CHOICE")
{
    m_name = name; 
    m_content = std::move(content);

    Frost::clamp_uint16_to_maximum(initial_cursor_index, content.size());
    Frost::clamp_uint16_to_maximum(initial_selected_index, content.size());

    m_cursor_index = initial_cursor_index;
    m_selected_index = initial_selected_index;
}


// Public

void UIChoice::render_no_status() const 
{
    m_coh.add_str("   " + m_name + ": ");
    m_coh.add_str(get_choice().content, get_choice().color);
}

void UIChoice::render_hovered() const 
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_name + ": ");
    m_coh.add_str(get_choice().content, get_choice().color);
}

void UIChoice::render_selected() const
{
    // Save the anchor so it can be restored after this method is finished.
    uint16_t previous_anchor = m_coh.get_anchor();

    m_coh.set_anchor_here();

    m_coh.add_str("   " + m_name + ": ");

    // Render choices before the cursor index.
    for(int i = 0; i < m_cursor_index; ++i)
    {
        m_coh.add_str("\n      " + m_content.at(i).content, m_content.at(i).color);
    }

    // Render hovered choice.
    m_coh.add_str("\n    > ", m_cursor_color);
    m_coh.add_str(m_content.at(m_cursor_index).content, m_content.at(m_cursor_index).color);

    // Render choices after the cursor index.
    for(int i = m_cursor_index + 1; i < m_content.size(); ++i)
    {
        m_coh.add_str("\n      " + m_content.at(i).content, m_content.at(i).color);
    }

    // Reset anchor to what it originally was before this method.
    m_coh.set_anchor(previous_anchor);
}

void UIChoice::add_choice(ColorString& new_choice) { m_content.push_back(std::move(new_choice)); }

UIItem::Status UIChoice::handle_input() 
{
    const std::vector<Key>& keys = InputHandler::get_pressed_and_available_keys();

    if(keys.size() == 0) return SELECTED;

    switch(keys.at(0))
    {
        // Select the hovered choice.
        case SDLK_RETURN:

            m_selected_index = m_cursor_index;

            InputHandler::block_key_until_released(SDLK_RETURN);
            return HOVERED;

        // Flag this UIChoice as deselected.
        case SDLK_f:

            InputHandler::block_key_until_released(SDLK_f);
            return HOVERED;

        case SDLK_w:    

            if(InputHandler::is_key_pressed_and_available(SDLK_LSHIFT))
            {   
                m_cursor_index = 0;
                InputHandler::delay_key(SDLK_w);
                return SELECTED;
            }

            // Deduct the cursor if it is not at 0.
            m_cursor_index -= (m_cursor_index != 0);

            InputHandler::delay_key(SDLK_w);

            return SELECTED;

        case SDLK_s:    

            if(InputHandler::is_key_pressed_and_available(SDLK_LSHIFT))
            {
                m_cursor_index = m_content.size() - 1;
                InputHandler::delay_key(SDLK_s);
                return SELECTED;
            }

            // Increment the cursor if it is not at the end of the list.
            m_cursor_index += (m_cursor_index != m_content.size() - 1);

            InputHandler::delay_key(SDLK_s);

            return SELECTED;
    }

    return SELECTED;
}

const ColorString& UIChoice::get_choice() const { return m_content.at(m_selected_index); }


// Private


