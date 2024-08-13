#include "UIText.hpp"
#include "InputHandler.hpp"
#include "Fr_StringManip.hpp"


// Constructors / Deconstructor

UIText::UIText(ConsoleOutputHandler& coh, std::string& cursor_color, std::string content) 
    : UIItem(coh, cursor_color, UIItem::TEXT) { m_content = content; }

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

UIItem::Status UIText::handle_input() 
{
    if(InputHandler::is_key_pressed_and_available(SDLK_RETURN)) 
    {
        InputHandler::block_key_until_released(SDLK_RETURN);

        // Return hovered, as the enter(return) key pressed here is deselecting the item.   
        return UIItem::HOVERED;
    }

    Frost::handle_input_for_string_manipulation(m_content);
}
