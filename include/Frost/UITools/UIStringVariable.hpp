/**
 * @file UIStringVariable.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class file. 
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "UIItem.hpp"


class UIStringVariable : public UIItem
{

public:

    UIStringVariable();

    UIStringVariable(ConsoleOutputHandler* coh, std::string* cursor_color, std::string name,
        std::string content = "", std::string default_content = "");

    void _render_no_status() const final;

    void _render_hovered() const final;

    void _render_selected() const final;

    Status _handle_input() final;

private:

    // Members

    // Name of this variable.
    std::string m_name;

    // Content of the variable, the value that the user sees and interacts with.
    std::string m_content;

    /** m_content will be set to this string if it is left empty after user deselection of this 
     * item. */
    std::string m_default_content;
};
