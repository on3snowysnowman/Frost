#include "Window.hpp"
#include "ProgramOutputHandler.hpp"


// Constructors / Deconstructor

Window::Window() {}

Window::Window(TextureHandler* tex_handler, uint16_t start_x, uint16_t start_y, uint16_t end_x, 
    uint16_t end_y, bool support_sprites)
{
    m_coh = new ConsoleOutputHandler(tex_handler);

    if(support_sprites) m_sp_handler = new SpriteHandler(tex_handler);
    else m_sp_handler = nullptr;
}


// Public

void Window::clear() { m_coh->clear_buffered_content(); }

void Window::render()
{
    m_coh->render();
    m_sp_handler->render();
}

void Window::add_ch(char c) {}

void Window::add_ch(char c, std::string color) {}

void Window::add_str(std::string str) {}

void Window::add_str(std::string str, std::string color) {}

void Window::add_new_line(uint8_t num) {}


// Private

void Window::_resize_window(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
    // If the window dimensions are not valid.
    if(start_x > end_x || start_y > end_y)
    {
        ProgramOutputHandler::log("Window::_resize_window() -> Attempted to create a window with "
            "invalid dimensions", Frost::ERR);
        exit(1);
    }

    m_start_x = start_x;
    m_start_y = start_y;
    m_end_x = end_x;
    m_end_y = end_y;

    m_coh->resize_dimensions(m_start_x + BORDER_SIZE, m_start_y + BORDER_SIZE, 
        m_end_x - BORDER_SIZE, m_end_y - BORDER_SIZE);
}
