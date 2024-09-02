/**
 * @file UIList.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for UIList.
 * @version 0.1
 * @date 24-08-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <initializer_list>
#include <vector>

#include "UIItem.hpp"


class UIList : public UIItem
{

public:

    UIList(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name,
        std::initializer_list<UIItem*> content, int16_t cursor_index = 0,
        int16_t selected_index = -1);

    void render_no_status() const final;

    void render_hovered() const final;

    void render_selected() const final;

    Status handle_input() final;

private:

    //Index of the cursor inside m_content.
    int16_t m_cursor_index;

    /** Index of the current selected item inside m_content. If this value is -1 there is no item 
     * selected. */
    int16_t m_selected_index;

    // Name of this List
    std::string m_name;

    // Items contained in the UIList.
    std::vector<UIItem*> m_content;

};

