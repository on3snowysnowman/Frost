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

UIIntVariable::UIIntVariable() {}

UIIntVariable::UIIntVariable(ConsoleOutputHandler* coh, std::string* cursor_color, 
    std::string _name, std::string _content, std::string _default_content) :
    UIItem(coh, cursor_color, "INT_VARIABLE")
{
    name = _name;
    content = _content;
    default_content = _default_content;

    _check_default_content_on_init();
    _check_content_on_init();
}


// Public

void UIIntVariable::_render_no_status() const 
{
    m_coh->add_str("   " + name + ": " + content);
}

void UIIntVariable::_render_hovered() const 
{
    m_coh->add_str(" > ", *m_cursor_color);
    m_coh->add_str(name + ": " + content);
}

void UIIntVariable::_render_selected() const 
{
    m_coh->add_str(" > ", *m_cursor_color);
    m_coh->add_str(name + ": ");
    m_coh->add_str(content + '_', *m_cursor_color);
}

UIItem::Status UIIntVariable::_handle_input() 
{
    if(InputHandler::is_key_pressed_and_available(SDLK_RETURN))
    {
        // Enter key was pressed as an input, deselect this item.
        
        InputHandler::block_key_until_released(SDLK_RETURN);

        // Remove any proceeding zeros in the content.
        Frost::remove_first_zeros(content);

        // If the content is empty, set it to the default content.
        if(content.size() == 0) content = default_content;

        // Flag this item as deselected by returning the HOVERED status.
        return HOVERED;
    }

    for(const Key& key : InputHandler::get_raw_pressed_keys())
    {
        // Valid integer.
        if(key >= '0' && key <= '9')
        {
            content.push_back(char(key));
            continue;
        }

        else if(content.size() == 0 && key == SDLK_MINUS)
        {
            content.push_back('-');
            continue;
        }

        else if(content.size() != 0 && key == SDLK_BACKSPACE)
        {
            // Backspace + Shift pressed, clear the content.
            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                content = "";
                continue;
            }

            content.pop_back();
        }
    }

    // Return the SELECTED status, as this item will continue to be selected until the user presses
    // the Enter key.
    return SELECTED;
}


// Private


void UIIntVariable::_check_content_on_init() 
{
    if(content.size() == 0)
    {
        // No need to check the content if it's empty. Set it to the default and return.
        content = default_content;
        return;
    }

    Frost::remove_first_zeros(content);

    // Iterate over each character and check if it is a valid integer. During this process, the 
    // initial proceeding zeros in front of the integer are removed.
    for(int i = 0; i < content.size(); ++i)
    {
        // Valid integer.
        if(content.at(i) >= '0' && content.at(i) <= '9') continue;

        // Invalid integer

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIIntVariable::_check_default_content_on_init(): Attempted to "
            "create an IntVariable with invalid content.", Frost::WARN);
        #endif

        content = default_content;
        break;
    }


}

void UIIntVariable::_check_default_content_on_init() 
{
    Frost::remove_first_zeros(default_content);

    // Iterate over each character and check if it is a valid integer.
    for(const char& c : default_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;

        // Invalid integer

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIIntVariable::_check_default_content_on_init(): Attempted to "
            "create an IntVariable with invalid default content.", Frost::WARN);
        #endif

        default_content = "";
        break;
    }
}
