/**
 * @file UIIntVariable.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation for UIIntVariable.
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "UIItem.hpp"


class UIIntVariable : public UIItem
{

public:

    UIIntVariable();

    UIIntVariable(ConsoleOutputHandler* coh, std::string* cursor_color, std::string name,
        std::string content = "0", std::string default_content = "0");

    void _render_no_status() const final;

    void _render_hovered() const final;

    void _render_selected() const final;

    Status _handle_input() final;  

    // Name of this variable.
    std::string m_name;

    // Content of the variable, the value that the user sees and interacts with.
    std::string m_content;

    /** m_content will be set to this string if it is left empty after user deselection of this 
     * item. */
    std::string m_default_content;

    /** 
     * @brief Checks m_content after construction, iterating through each 
     * character to see if they are all valid integers.
    */
    void _check_content_on_init();

    /** 
     * @brief Checks m_default_content after construction, iterating through 
     * each character to see if they are all valid integers.
     * 
    */
    void _check_default_content_on_init();
};
