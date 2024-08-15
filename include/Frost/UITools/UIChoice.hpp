/**
 * @file UIChoice.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for UIChoice.
 * @version 0.1
 * @date 24-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <initializer_list>
#include <cstdint>
#include <vector>

#include "UIItem.hpp"
#include "ColorString.hpp"


class UIChoice : public UIItem
{

public:

    // Requires parameters for construction.
    UIChoice() = delete;

    /** @note "content" is moved with std::move and will have its ownership transferred. */
    UIChoice(ConsoleOutputHandler& coh, std::string& cursor_color, std::string name,
        std::initializer_list<ColorString> content, uint16_t initial_cursor_index = 0, 
        uint16_t initial_selected_index = 0);

    void render_no_status() const final;

    void render_hovered() const final;

    void render_selected() const final;

    /** Adds another choice to the content.
     * 
     * @note "new_choice" is moved with std::move and will have its ownership transferred.
     * 
     * @param new_choice Choice to add.
     */
    void add_choice(ColorString& new_choice);

    Status handle_input() final;

    /** Returns a const reference to the ColorString object that is currently selected.
     */
    const ColorString& get_choice() const;

private:

    // Name of the choice.
    std::string m_name;

    // Index of the cursor inside m_content.
    uint16_t m_cursor_index;

    /** Index of the current selected item inside m_content. If this value is -1 there is no item
     * selected.
     */
    uint16_t m_selected_index;

    std::vector<ColorString> m_content;
};
