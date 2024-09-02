/**
 * @file UIList.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation for UIList.
 * @version 0.1
 * @date 24-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "UIList.hpp"
#include "InputHandler.hpp"


// Constructors / Deconstructor

UIList::UIList(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name,
    std::initializer_list<UIItem*> content, int16_t cursor_index, 
    int16_t selected_index) : UIItem(coh, cursor_color, "LIST")
{
    for(UIItem* item : content)
    {
        m_content.push_back(item);
    }

    m_cursor_index = cursor_index;
    m_selected_index = selected_index;

    m_name = name;
}


// Public

void UIList::render_no_status() const
{
    if(m_content.size() == 0) 
    {
        m_coh.add_str("    " + m_name + ": ");
        m_coh.add_str("Empty", "Red");
        return;
    }

    m_coh.add_str("   " + m_name + ": [...]");
}

void UIList::render_hovered() const 
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_name + ": [...]");
}

void UIList::render_selected() const 
{
    if(m_content.size() == 0)
    {
        m_coh.add_str(" > " + m_name, m_cursor_color);
        m_coh.add_str(": ");
        m_coh.add_str("Empty", "Red");
        return;
    }

    // Save the anchor so it can be restored after this method is finished.
    uint16_t previous_anchor = m_coh.get_anchor();

    m_coh.add_str("   ");

    m_coh.set_anchor_here();

    m_coh.add_str(m_name + ":");

    // Render Items before the cursor's position.
    for(int i = 0; i < m_cursor_index; ++i)
    {
        m_coh.add_new_line();
        m_content.at(i)->render_no_status();
    }

    // The Item at the cursor's position is selected.
    if(m_selected_index > -1)
    {
        m_coh.add_new_line();
        m_content.at(m_cursor_index)->render_selected();
    }

    // The Item is hovered by the cursor.
    else
    {
        m_coh.add_new_line();
        m_content.at(m_cursor_index)->render_hovered();
    }

    // Render Items after the cursor's position.
    for(int i = m_cursor_index + 1; i < m_content.size(); ++i)
    {
        m_coh.add_new_line();
        m_content.at(i)->render_no_status();
    }
    
    // Reset anchor to what it originally was before this method.
    m_coh.set_anchor(previous_anchor);
}

UIItem::Status UIList::handle_input() 
{
    // There is an item selected.
    if(m_selected_index > -1)
    {
        // Flag the selected item to handle input, and process the return status. If the return 
        // status is HOVERED, this item is no longer selected.
        if(m_content.at(m_selected_index)->handle_input() == HOVERED) m_selected_index = -1;
        return SELECTED;
    }

    // No item is selected.

    const std::vector<Key>& keys = InputHandler::get_pressed_and_available_keys();

    if(keys.size() == 0) return SELECTED;

    switch(*keys.begin())
    {
        case SDLK_RETURN:   

            // Select the item at the cursor's index.

            InputHandler::block_key_until_released(SDLK_RETURN);

            m_selected_index = m_cursor_index;
            return SELECTED;

        case SDLK_f:    

            // Deselect this UIList

            InputHandler::block_key_until_released(SDLK_f);

            return HOVERED;

        case SDLK_w:    

            InputHandler::delay_key(SDLK_w);

            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                m_cursor_index = 0;
                return SELECTED;
            }

            // Deduct 1 from the cursor index if it does not equal 0.
            m_cursor_index -= m_cursor_index != 0;
            return SELECTED;

        case SDLK_s:    

            InputHandler::delay_key(SDLK_s);

            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                m_cursor_index = m_content.size() - 1;
                return SELECTED;
            }

            // Add 1 to the cursor index if it is not at the end of the content.
            m_cursor_index += m_cursor_index != m_content.size() - 1; 
    }

    return SELECTED;
}


// Private


