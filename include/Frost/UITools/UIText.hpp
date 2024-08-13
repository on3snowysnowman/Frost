/**
 * @file UIText.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Derived Text component of UIItem.
 * @version 0.1
 * @date 24-08-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "UIItem.hpp"
#include "ConsoleOutputHandler.hpp"


class UIText : public UIItem
{

public:

    // Requires parameters for construction.
    UIText() = delete;

    UIText(ConsoleOutputHandler& coh, std::string& cursor_color, std::string content = "");

    void render_no_status() const final;
    void render_hovered() const final;
    void render_selected() const final;

    UIItem::Status handle_input() final;

private:

    // Content of the item, the text that the user sees and interacts with.
    std::string m_content;
};
