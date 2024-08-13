/**
 * @file UIVariable.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single Class Implementation.
 * @version 0.1
 * @date 24-08-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "UIVariable.hpp"
#include "Fr_StringManip.hpp"
#include "InputHandler.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif


// Constructors / Deconstructor

UIVariable::UIVariable(ConsoleOutputHandler& coh, std::string& cursor_color, 
    VariableType var_type, std::string name, std::string content, std::string default_content)
    : UIItem(coh, cursor_color, UIItem::VARIABLE), m_NAME(name)
{
    m_var_type = var_type;
    m_content = content;
    m_default_content = default_content;

    switch(m_var_type)
    {
        case STRING:    

            m_targ_input_handling_function = &_handle_string_input;
            break;

        case INT:   

            m_targ_input_handling_function = &_handle_int_input;
            break;

        case FLOAT:

            m_targ_input_handling_function = &_handle_float_input;
            break;
    }

    _check_content_on_init();
}


// Public

void UIVariable::render_no_status() const 
{
    m_coh.add_str("   " + m_NAME + ": " + m_content);
}

void UIVariable::render_hovered() const
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_NAME + ": " + m_content);
}

void UIVariable::render_selected() const
{
    m_coh.add_str(" > " + m_NAME + ": " + m_content + '_', m_cursor_color);
}

UIItem::Status UIVariable::handle_input() 
{
    // Call the respective input handling function and return the status that it returns.
    return (this->*m_targ_input_handling_function)();
}


// Private

void UIVariable::_check_content_on_init()
{
    // No checks for type STRING are needed, since for Variables of type STRING can contain any 
    // character.
    if(m_var_type == STRING) return;

    switch(m_var_type)
    {
        case INT:   

            _check_content_for_int_type();
            break;

        case FLOAT:

            _check_content_for_float_type();
            break;
    }
}

void UIVariable::_check_content_for_int_type() 
{
    // Check m_content for invalid characters.
    for(char c : m_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;

        // Invalid integer.    

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIVariable::_check_content_for_int_type(): Attempted to "
            "create UIVariable of type INT with a content contained with invalid characters.",
            Frost::WARN);
        #endif

        m_content = "";
        break;
    
    }

    if(m_default_content.size() == 0)
    {
        // There is nothing in the default content, therefore the checks for its contents can be 
        // skipped.
        m_default_content.push_back('0');
        return;
    }

    // Check m_default_content for invalid characters.
    for(char c : m_default_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;
        
        // Invalid integer.

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIVariable::_check_content_for_int_type(): Attempted to "
            "create UIVariable of type INT with a default content contained with invalid "
            "characters.", Frost::WARN);
        #endif

        m_default_content = "";
        return;
    }
}

void UIVariable::_check_content_for_float_type() 
{
    // Check m_content for invalid characters.
    for(char c : m_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;

        // Invalid character.
        if(c != '.') 
        {
            #ifdef FROST_DEBUG

            ProgramOutputHandler::log("UIVariable::_check_content_for_float_type(): Attempted "
                "to create UIVariable of type FLOAT with a content contained with "
                "invalid characters.", Frost::WARN);
            #endif

            m_content = "";
            break;
        }

        // m_content does not contain a decimal yet.
        if(!m_has_decimal_point)
        {
            m_has_decimal_point = true;
            continue;
        }

        // m_content already contains a decimal, and another was invalidly attempted to be added.

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIVariable::_check_content_for_float_type(): Attempted "
            "to create UIVariable of type FLOAT with a content contained with "
            "multiple decimal points.", Frost::WARN);
        #endif

        m_content = "";
        break;
    }

    if(m_default_content.size() == 0)
    {
        // There is nothing in the default content, therefore the checks for its contents can be 
        // skipped.
        m_default_content.push_back('0');
        return;
    }

    // Check m_default_content for invalid characters.
    for(char c : m_default_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;

        // Invalid character.
        if(c != '.') 
        {
            #ifdef FROST_DEBUG

            ProgramOutputHandler::log("UIVariable::_check_content_for_float_type(): Attempted "
                "to create UIVariable of type FLOAT with a default content contained with "
                "invalid characters.", Frost::WARN);
            #endif

            m_content = "";
            break;
        }

        // m_content does not contain a decimal yet.
        if(!m_has_decimal_point)
        {
            m_has_decimal_point = true;
            continue;
        }

        // m_content already contains a decimal, and another was invalidly attempted to be added.

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("UIVariable::_check_content_for_float_type(): Attempted "
            "to create UIVariable of type FLOAT with a default content contained with "
            "multiple decimal points.", Frost::WARN);
        #endif

        m_content = "";
        break;
    }

}

bool UIVariable::_check_and_handle_deselect()
{
    if(InputHandler::is_key_pressed_and_available(SDLK_RETURN))
    {
        InputHandler::block_key_until_released(SDLK_RETURN);

        // If the content was left empty when deselected, set it to the default content.
        if(m_content.size() == 0) m_content = m_default_content;

        return true;
    }

    return false;
}

UIItem::Status UIVariable::_handle_string_input()
{
    // If the user deselected this item, return that this item is now HOVERED.
    if(_check_and_handle_deselect()) return UIItem::HOVERED;

    Frost::handle_input_for_string_manipulation(m_content);

    return UIItem::SELECTED;
}

UIItem::Status UIVariable::_handle_int_input()
{
    // If the user deselected this item, return that this item is now HOVERED.
    if(_check_and_handle_deselect()) return UIItem::HOVERED;

    for(const Key& key : InputHandler::get_raw_pressed_keys())
    {
        /// If the key is an integer
        if(key >= '0' && key <= '9') 
        {
            m_content.push_back(key); 
            continue;
        }

        if(key == SDLK_BACKSPACE)
        {
            if(m_content.size() > 0) m_content.pop_back();
            continue;
        }
    }

    return UIItem::SELECTED;
}

UIItem::Status UIVariable::_handle_float_input()
{
    // If the user deselected this item, return that this item is now HOVERED.
    if(_check_and_handle_deselect()) return UIItem::HOVERED;

    for(const Key& key : InputHandler::get_raw_pressed_keys())
    {
        // If the key is an integer
        if(key >= '0' && key <= '9') 
        {
            m_content.push_back(key); 
            continue;
        }

        switch(key)
        {
            case '.':

                if(!m_has_decimal_point)
                {
                    m_has_decimal_point = true;
                    m_content.push_back('.');
                }

                continue;

            case SDLK_BACKSPACE:    

                if(m_content.size() == 0) continue;

                if(*(--m_content.end()) == '.')
                {
                    m_content.pop_back();
                    m_has_decimal_point = false;
                }
                
                break;
        }
    }

    return UIItem::SELECTED;
}
