/**
 * @file UITextList.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation for UITextList.
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "UITextList.hpp"
#include "Fr_Math.hpp"
#include "InputHandler.hpp"
#include "Fr_StringManip.hpp"


// Constructors / Deconstructor

UITextList::UITextList(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name,
    std::initializer_list<std::string> content, int16_t cursor_position, 
    int16_t selected_position) : UIItem(coh, cursor_color, "TEXT_LIST")
{
    m_name = name;
    
    for(const std::string& str : content)
    {
        m_content.push_back(std::move(str));
    }

    m_content.push_back(std::string{"Add Text"});

    Frost::clamp_int16_to_maximum(m_cursor_index, m_content.size());
    Frost::clamp_int16_to_maximum(m_selected_index, m_content.size());

    m_cursor_index = cursor_position;
    m_selected_index = selected_position;
}


// Public

void UITextList::render_no_status() const 
{
    m_coh.add_str("   " + m_name + ": [...]");
}

void UITextList::render_hovered() const 
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_name + ": [...]");
}

void UITextList::render_selected() const 
{
    // Save the anchor so it can be restored after this method is finished.
    uint16_t previous_anchor = m_coh.get_anchor();

    m_coh.set_anchor_here();

    m_coh.add_str("   " + m_name + ":");

    // Render Text before selected cursor index.
    for(int i = 0; i < m_cursor_index; ++i)
    {
        m_coh.add_str("\n      " + m_content.at(i));
    }

    // If the Text at the cursor's position is selected.
    if(m_selected_index > -1)
    {
        // Render this text item as selected.
        
        m_coh.add_str("\n    > " + m_content.at(m_cursor_index) + '_', m_cursor_color);
    }

    // This Text is only hovered.
    else
    {
        m_coh.add_str("\n    > ", m_cursor_color);
        m_coh.add_str(m_content.at(m_cursor_index));
    }

    // Render Text after the cursor's index.
    for(int i = m_cursor_index + 1; i < m_content.size(); ++i)
    {
        m_coh.add_str("\n      " + m_content.at(i));
    }

    // Reset anchor to what it originally was before this method.
    m_coh.set_anchor(previous_anchor);
}

UIItem::Status UITextList::handle_input() 
{
    // There is Text selected.
    if(m_selected_index > -1)
    {
        if(InputHandler::is_key_pressed_and_available(SDLK_RETURN))
        {
            // Deselect the Text that is selected by setting the selected index to -1.

            InputHandler::block_key_until_released(SDLK_RETURN);

            m_selected_index = -1;
            return SELECTED;
        }

        Frost::handle_input_for_string_manipulation(m_content.at(m_selected_index));

        return SELECTED;
    }

    // Nothing is selected.

    const std::vector<Key>& keys = InputHandler::get_pressed_and_available_keys();

    if(keys.size() == 0) return SELECTED;

    // Handle the first registered key that is pressed.
    switch(InputHandler::get_pressed_and_available_keys().at(0))
    {
        case SDLK_RETURN:

            InputHandler::block_key_until_released(SDLK_RETURN);

            // If the "Add Text" button was selected
            if(m_cursor_index == m_content.size() - 1)
            {
                m_content.insert(m_content.begin() + m_cursor_index, "Text");
                ++m_cursor_index;
                return SELECTED;
            }

            m_selected_index = m_cursor_index;

            return SELECTED;

        // Flag this UITextList as deselected.
        case SDLK_f:    

            InputHandler::block_key_until_released(SDLK_f);

            return HOVERED;

        case SDLK_BACKSPACE:    

            InputHandler::block_key_until_released(SDLK_BACKSPACE);

            // If the attempted deleted item is the "Add Text" button.
            if(m_cursor_index == m_content.size() - 1) return SELECTED;

            // Erase the item at the cursor's position.
            m_content.erase(m_content.begin() + m_cursor_index);

            m_cursor_index -= m_cursor_index != 0;

            return SELECTED;

        case SDLK_w:

            InputHandler::delay_key(SDLK_w);

            m_cursor_index -= m_cursor_index != 0;

            return SELECTED;;

        case SDLK_s:

            InputHandler::delay_key(SDLK_s);

            m_cursor_index += m_cursor_index != m_content.size() - 1;

            return SELECTED;;
    }

    return SELECTED;
}
