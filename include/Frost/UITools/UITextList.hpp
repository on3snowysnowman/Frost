/**
 * @file UITextList.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for UITextList.
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <initializer_list>

#include "UIItem.hpp"
#include "UIText.hpp"


class UITextList : public UIItem
{

public:

    // Requires parameters for construction.

    UITextList() = delete;

    UITextList(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name, 
        std::initializer_list<std::string> content, int16_t cursor_index = 0, 
        int16_t selected_index = -1);

    void render_no_status() const final;

    void render_hovered() const final;

    void render_selected() const final;

    void add_item(UIItem* item);

    Status handle_input() final;

private:

    // Index of the cursor inside m_content.
    int16_t m_cursor_index;

    /** Index of the current selected item inside m_content. If this value is -1 there is no item 
     * selected. */
    int16_t m_selected_index;

    // Name of this TextList.
    std::string m_name;

    // Content of the list, a vector of UIText that the user can parse and select.
    std::vector<std::string> m_content; 
};
