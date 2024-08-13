#include "UIItem.hpp"


// Constructors / Deconstructor

UIItem::UIItem(ConsoleOutputHandler& coh, std::string& cursor_color, 
    UIItem::ItemType item_type) : m_ITEM_TYPE(item_type), m_coh(coh), 
    m_cursor_color(cursor_color) {}


// Public

void UIItem::render_no_status() const {}

void UIItem::render_hovered() const {}

void UIItem::render_selected() const {}
