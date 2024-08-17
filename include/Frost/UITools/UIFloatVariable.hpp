/**
 * @file UIFloatVariable.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for UIFloatVariable.
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "UIItem.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif


class UIFloatVariable : public UIItem
{

public:

    // Requires parameters for construction.
    UIFloatVariable() = delete;

    UIFloatVariable(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name,
        std::string content = "0", std::string default_content = "0");

    void render_no_status() const final;

    void render_hovered() const final;

    void render_selected() const final;

    Status handle_input() final;

private:

    // Members

    // If a decimal point currently exists inside m_content.
    bool m_content_has_decimal = false;

    // If a decimal point exists m_default_content.
    bool m_default_content_has_decimal = false;

    // Name of this variable.
    std::string m_name;

    // Content of the variable, the value that the user sees and interacts with.
    std::string m_content;

    /** m_content will be set to this string if it is left empty after user deselection of this 
     * item. */
    std::string m_default_content;


    // Methods

    /** Checks m_content after construction, iterating through each character to see if they are
     * all valid integers.
    */
    void _check_content_on_init();

     /** Checks m_default_content after construction, iterating through each character to see if they are
     * all valid integers.
    */
    void _check_default_content_on_init();
};

