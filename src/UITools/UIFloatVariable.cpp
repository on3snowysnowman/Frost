/**
 * @file UIFloatVariable.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "UIFloatVariable.hpp"
#include "InputHandler.hpp"
#include "Fr_StringManip.hpp"


// Constructors / Deconstructor

UIFloatVariable::UIFloatVariable(ConsoleOutputHandler& coh, std::string& cursor_color, 
    std::string name, std::string content, std::string default_content) :
    UIItem(coh, cursor_color, "FLOAT_VARIABLE")
{
    m_name = name;
    m_content = content;
    m_default_content = default_content;

    _check_default_content_on_init();
    _check_content_on_init();
}


// Public

void UIFloatVariable::render_no_status() const
{
    m_coh.add_str("   " + m_name + ": " + m_content);
}

void UIFloatVariable::render_hovered() const
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_name + ": " + m_content);
}

void UIFloatVariable::render_selected() const
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_name + ": ");
    m_coh.add_str(m_content + '_', m_cursor_color);
}


UIItem::Status UIFloatVariable::handle_input()
{
    if(InputHandler::is_key_pressed_and_available(SDLK_RETURN))
    {
        // Enter key was pressed as an input, deselect this item.
        
        InputHandler::block_key_until_released(SDLK_RETURN);

        // Remove any proceeding zeros in the content.
        Frost::remove_first_zeros(m_content);

        // If the content is empty, set it to the default content.
        if(m_content.size() == 0) 
        {
            m_content = m_default_content;
            m_content_has_decimal = m_default_content_has_decimal;

            // Flag this item as deselected by returning the HOVERED status.
            return HOVERED;
        }

        // If there is a decimal point in the content, and the character at the beginning of the
        // string is the decimal, add a 0 in front of it as it was just removed in the 
        // remove_first_zeros() call.
        if(m_content_has_decimal && *(m_content.begin()) == '.')
        {
            m_content.insert(m_content.begin(), '0');
        }

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

        else if(key == '.')
        {
            // If the content already has a decimal point.
            if(m_content_has_decimal) continue;

            m_content.push_back('.');
            m_content_has_decimal = true;
            continue;
        }

        else if(m_content.size() != 0 && key == SDLK_BACKSPACE)
        {
            // Backspace + Shift pressed, clear the content.
            if(InputHandler::is_key_pressed(SDLK_LSHIFT))
            {
                m_content = "";
                m_content_has_decimal = false;
                continue;
            }

            // Character to be deleted is the decimal point.
            if(*(--m_content.end()) == '.') m_content_has_decimal = false;

            m_content.pop_back();
        }
    }

    // Return the SELECTED status, as this item will continue to be selected until the user presses
    // the Enter key.
    return SELECTED;
}


// Private

void UIFloatVariable::_check_content_on_init() 
{
    if(m_content.size() == 0)
    {
        m_content = m_default_content;
        m_content_has_decimal = m_default_content_has_decimal;
        return;
    }

   Frost::remove_first_zeros(m_content);

    // Iterate over each character and check if it is a valid integer.
    for(const char& c : m_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;

        else if(c == '.')
        {
            // If no decimal point exists yet.
            if(!m_content_has_decimal)
            {
                m_content_has_decimal = true;
                continue;
            }

            // Decimal point exists already.
            #ifdef FROST_DEBUG

            ProgramOutputHandler::log("UIFloatVariable::_check_content_on_init(): Content "
                "contains multiple decimal points.", Frost::WARN);
            #endif

            m_content = m_default_content;
            m_content_has_decimal = m_default_content_has_decimal;
            break;
        }

        // Invalid character

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIFloatVariable::_check_content_on_init(): Attempted to "
            "create a FloatVariable with invalid content.", Frost::WARN);
        #endif

        m_content = m_default_content;
            m_content_has_decimal = m_default_content_has_decimal;
        break;
    }

        if(m_content_has_decimal && 
            *(--m_content.begin()) == '.') m_content.insert(m_content.begin(), '0');
}

void UIFloatVariable::_check_default_content_on_init() 
{
    Frost::remove_first_zeros(m_default_content);

    // Iterate over each character and check if it is a valid integer.
    for(const char& c : m_default_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;

        else if(c == '.')
        {
            // If no decimal point exists yet.
            if(!m_default_content_has_decimal)
            {
                m_default_content_has_decimal = true;
                continue;
            }

            // Decimal point exists already.
            #ifdef FROST_DEBUG

            ProgramOutputHandler::log("UIFloatVariable::_check_default_content_on_init(): Default "
                "content contains multiple decimal points.", Frost::WARN);
            #endif

            m_default_content = "";
            m_default_content_has_decimal = false;
            break;
        }

        // Invalid character

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIFloatVariable::_check_default_content_on_init(): Attempted to "
            "create a FloatVariable with invalid default content.", Frost::WARN);
        #endif

        m_default_content = "";
        m_default_content_has_decimal = false;
        break;
    }

        if(m_default_content_has_decimal && *(--m_default_content.begin()) == '.') 
            m_default_content.insert(m_default_content.begin(), '0');
}


