#include "UIItem.hpp"


// Constructors / Deconstructor

UIItem::UIItem(ConsoleOutputHandler& coh, Frost::UIItemType item_type) 
    : m_ITEM_TYPE(item_type), m_coh(coh) {}


// Public

void UIItem::render_no_status() const {}

void UIItem::render_hovered() const {}

void UIItem::render_selected() const {}
