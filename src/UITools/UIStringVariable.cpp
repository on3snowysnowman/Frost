/**
 * @file UIStringVariable.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation for a single class file.
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "UIStringVariable.hpp"
#include "InputHandler.hpp"
#include "Fr_StringManip.hpp"


// Constructors / Deconstructor

UIStringVariable::UIStringVariable(ConsoleOutputHandler& coh, std::string& cursor_color, 
    std::string name, std::string content, std::string default_content) : 
    UIItem(coh, cursor_color, "STRING_VARIABLE")
{
    m_name = name;

    m_content = content;
    m_default_content = default_content;
}


// Public

void UIStringVariable::render_no_status() const 
{
    m_coh.add_str("   " + m_name + ": " + m_content);
};

void UIStringVariable::render_hovered() const 
{
    m_coh.add_str(" > ", m_cursor_color); 
    m_coh.add_str(m_name + ": " + m_content);
};

void UIStringVariable::render_selected() const 
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_name + ": ");
    m_coh.add_str(m_content + '_', m_cursor_color);
};

UIItem::Status UIStringVariable::handle_input() 
{
    if(InputHandler::is_key_pressed_and_available(SDLK_RETURN))
    {
        // Enter key was pressed as an input, deselect this item.

        // If the content was left empty on deselection, set it to the default content.
        if(m_content.size() == 0) m_content = m_default_content;

        InputHandler::block_key_until_released(SDLK_RETURN);
        
        // Flag this item as deselected by returning the HOVERED status.
        return HOVERED;
    }

    Frost::handle_input_for_string_manipulation(m_content);

    // Return the SELECTED status, as this item will continue to be selected until the user presses
    // the Enter key.
    return SELECTED;
}

