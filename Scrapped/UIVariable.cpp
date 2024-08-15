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
    // Variables of type STRING do not require checking since all characters are valid.'
    if(m_var_type == STRING) return;

    else if(m_var_type == INT) 
    {
        _check_content_for_int_type();
        return;
    }

    // Content is of FLOAT type.

    _check_content_for_float_type();
}

void UIVariable::_check_content_for_int_type()
{
    if(m_default_content.size() != 0)
    {
        // Check default content.
        for(char c : m_default_content)
        {
            // Valid integer
            if(c >= '0' && c <= '9') continue;

            // else : Non valid integer.

            #ifdef FROST_DEBUG:
            ProgramOutputHandler("UIVariable::_check_content_for_int_type(): Attempted to create "
                "variable with a default content of invalid characters.", Frost::WARN);
            #endif

            m_default_content = "";
            break;
        }
    }

    // Check content.
    for(char c : m_content)
    {
        // Valid integer
        if(c >= '0' && c <= '9') continue;

        // else : Non valid integer.

        #ifdef FROST_DEBUG:
        ProgramOutputHandler("UIVariable::_check_content_for_int_type(): Attempted to create "
            "variable with a content of invalid characters.", Frost::WARN);
        #endif

        m_content = m_default_content;
        break;
    }
}

void UIVariable::_check_content_for_float_type()
{
    if(m_default_content.size() != 0)
    {
        // Check default content.
        for(char c : m_default_content)
        {
            // Valid integer.
            if(c >= '0' && c <= '9') continue;

            // The only non integer character that can be added to a float is a decimal point.
            if(c != '.')
            {
                if(!m_default_has_decimal_point)
                {
                    m_default_has_decimal_point;
                    continue;
                }

                // A decimal point already exists, and is being attempted to be added again.

                #ifdef FROST_DEBUG:
                ProgramOutputHandler("UIVariable::_check_content_for_float_type(): Attempted to create "
                    "variable with default content containing multiple decimal points.", Frost::WARN);
                #endif

                m_default_content = "";
                m_default_has_decimal_point = false;
                break;
            }

            // Non valid integer and not a decimal point.

            #ifdef FROST_DEBUG:
            ProgramOutputHandler("UIVariable::_check_content_for_float_type(): Attempted to create "
                "variable with a default content of invalid characters.", Frost::WARN);
            #endif

            m_default_content = "";
            m_default_has_decimal_point = false;
            break;
        }
    }

    // Check content.
    for(char c : m_content)
    {
        // Valid integer.
        if(c >= '0' && c <= '9') continue;

        // The only non integer character that can be added to a float is a decimal point.
        if(c != '.')
        {
            if(!m_has_decimal_point)
            {
                m_has_decimal_point;
                continue;
            }

            // A decimal point already exists, and is being attempted to be added again.

            #ifdef FROST_DEBUG:
            ProgramOutputHandler("UIVariable::_check_content_for_float_type(): Attempted to create "
                "variable with content containing multiple decimal points.", Frost::WARN);
            #endif

            m_content = m_default_content;
            m_has_decimal_point = m_default_has_decimal_point;
            break;
        }

        // Non valid integer and not a decimal point.

        #ifdef FROST_DEBUG:
        ProgramOutputHandler("UIVariable::_check_content_for_float_type(): Attempted to create "
            "variable with a content of invalid characters.", Frost::WARN);
        #endif

        m_content = m_default_content;
        m_has_decimal_point = m_default_has_decimal_point;
        break;
    }
}


