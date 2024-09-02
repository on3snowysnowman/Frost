/**
 * @file UIIntVariable.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation for UIIntVariable.
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "UIIntVariable.hpp"
#include "InputHandler.hpp"
#include "Fr_StringManip.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif


// Constructors / Deconstructor

UIIntVariable::UIIntVariable(ConsoleOutputHandler& coh, std::string& cursor_color, 
    std::string name, std::string content, std::string default_content) :
    UIItem(coh, cursor_color, "INT_VARIABLE")
{
    m_name = name;
    m_content = content;
    m_default_content = default_content;

    _check_default_content_on_init();
    _check_content_on_init();
}


// Public

void UIIntVariable::render_no_status() const 
{
    m_coh.add_str("   " + m_name + ": " + m_content);
}

void UIIntVariable::render_hovered() const 
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_name + ": " + m_content);
}

void UIIntVariable::render_selected() const 
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_name + ": ");
    m_coh.add_str(m_content + '_', m_cursor_color);
}

UIItem::Status UIIntVariable::handle_input() 
{
    if(InputHandler::is_key_pressed_and_available(SDLK_RETURN))
    {
        // Enter key was pressed as an input, deselect this item.
        
        InputHandler::block_key_until_released(SDLK_RETURN);

        // Remove any proceeding zeros in the content.
        Frost::remove_first_zeros(m_content);

        // If the content is empty, set it to the default content.
        if(m_content.size() == 0) m_content = m_default_content;

        // Flag this item as deselected by returning the HOVERED status.
        return HOVERED;
    }

    for(const Key& key : InputHandler::get_raw_pressed_keys())
    {
        // Valid integer.
        if(key >= '0' && key <= '9')
        {
            m_content.push_back(char(key));
            continue;
        }

        else if(m_content.size() != 0 && key == SDLK_BACKSPACE)
        {
            // Backspace + Shift pressed, clear the content.
            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                m_content = "";
                continue;
            }

            m_content.pop_back();
        }
    }

    // Return the SELECTED status, as this item will continue to be selected until the user presses
    // the Enter key.
    return SELECTED;
}


// Private


void UIIntVariable::_check_content_on_init() 
{
    if(m_content.size() == 0)
    {
        // No need to check the content if it's empty. Set it to the default and return.
        m_content = m_default_content;
        return;
    }

    Frost::remove_first_zeros(m_content);

    // Iterate over each character and check if it is a valid integer. During this process, the 
    // initial proceeding zeros in front of the integer are removed.
    for(int i = 0; i < m_content.size(); ++i)
    {
        // Valid integer.
        if(m_content.at(i) >= '0' && m_content.at(i) <= '9') continue;

        // Invalid integer

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIIntVariable::_check_default_content_on_init(): Attempted to "
            "create an IntVariable with invalid content.", Frost::WARN);
        #endif

        m_content = m_default_content;
        break;
    }


}

void UIIntVariable::_check_default_content_on_init() 
{
    Frost::remove_first_zeros(m_default_content);

    // Iterate over each character and check if it is a valid integer.
    for(const char& c : m_default_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;

        // Invalid integer

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIIntVariable::_check_default_content_on_init(): Attempted to "
            "create an IntVariable with invalid default content.", Frost::WARN);
        #endif

        m_default_content = "";
        break;
    }
}
