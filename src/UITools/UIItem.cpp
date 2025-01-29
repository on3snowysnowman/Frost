#include "UIItem.hpp"


// Constructors / Deconstructor

UIItem::UIItem() 
{ 
    m_coh = nullptr; 
    m_cursor_color = nullptr;    
}

UIItem::UIItem(ConsoleOutputHandler* coh, std::string* cursor_color, 
    std::string item_type) : m_item_type(item_type), m_cursor_color(cursor_color), m_coh(coh) {}


// Public

void UIItem::render_no_status() const {}

void UIItem::render_hovered() const {}

void UIItem::render_selected() const {}

UIItem::Status UIItem::handle_input() { return HOVERED; }
