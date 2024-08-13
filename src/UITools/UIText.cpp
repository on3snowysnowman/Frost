#include "UIText.hpp"


// Constructors / Deconstructor

UIText::UIText(ConsoleOutputHandler& coh, std::string& cursor_color, std::string content) 
    : UIItem(coh, Frost::TEXT), m_cursor_color(cursor_color) { m_content = content; }

// Public

void UIText::render_no_status() const 
{
    m_coh.add_str("   " + m_content);
}

void UIText::render_hovered() const 
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_content);
}

void UIText::render_selected() const 
{
    m_coh.add_str(" > ", m_cursor_color);
    m_coh.add_str(m_content, m_cursor_color);
}

Frost::UIItemStatus UIText::handle_input() 
{
    
}

