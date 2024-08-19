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

    m_coh.add_str("   ");

    m_coh.set_anchor_here();

    m_coh.add_str(m_name + ": ");

    // Render Items before the cursor's position.
    for(int i = 0; i < m_cursor_index; ++i)
    {
        m_content.at(i)->render_no_status();
    }

    // The Item at the cursor's position is selected.
    if(m_selected_index > -1)
    {
        m_content.at(m_cursor_index)->render_selected();
    }

    // The Item is hovered by the cursor.
    else
    {
        m_content.at(m_cursor_index)->render_hovered();
    }


    // Render Items after the cursor's position.
    for(int i = m_cursor_index + 1; i < m_content.size(); ++i)
    {
        m_content.at(i)->render_no_status();
    }
}

UIItem::Status UIList::handle_input() 
{
    return SELECTED;
}


// Private


