#include "ConsoleOutputHandler.h"
#include "ProgramOutputHandler.h"

// Constructors / Deconstructor

ConsoleOutputHandler::ConsoleOutputHandler() {}

ConsoleOutputHandler::ConsoleOutputHandler(TextureHandler* texture_handler) 
{ 
    m_text_ren_handler = TextRenderingHandler(texture_handler);
    m_text_ren_handler.set_size_scale(2.0);

    m_font_scaled_width = m_text_ren_handler.get_scaled_font_width();
    m_font_scaled_height = m_text_ren_handler.get_scaled_font_height(); 
    resize_dimensions(0, 0, 0, 0);
}

ConsoleOutputHandler::ConsoleOutputHandler(TextureHandler* texture_handler, uint16_t start_x, 
    uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
    m_text_ren_handler = TextRenderingHandler(texture_handler);
    m_text_ren_handler.set_size_scale(2.0);
    
    m_font_scaled_width = m_text_ren_handler.get_scaled_font_width();
    m_font_scaled_height = m_text_ren_handler.get_scaled_font_height(); 
    resize_dimensions(start_x, start_y, end_x, end_y);
}


// Public

void ConsoleOutputHandler::resize_dimensions(uint16_t start_x, uint16_t start_y, uint16_t end_x, 
    uint16_t end_y)
{
    if(end_x < start_x || end_y < start_y)
    {
        ProgramOutputHandler::log("ConsoleOutputHandler.resize_dimensions() -> Attempted to create"
            " with invalid dimensions. Setting dimensions to 0. ", Frost::WARN);
        start_x = 0;
        start_y = 0;
        end_x = 0;
        end_y = 0;

        return;
    }

    // Set the screen boundaries.
    m_start_x = start_x;
    m_start_y = start_y;
    m_end_x = end_x;
    m_end_y = end_y;

    // Calculate screen width in characters. 
    m_screen_character_width = (end_x - start_x) / m_font_scaled_width;

    m_screen_character_height = (end_y - start_y) / (m_font_scaled_height * 
        VERTICAL_SPACE_MODIFIER);
}   

void ConsoleOutputHandler::move_cursor(uint16_t x, uint16_t y)
{
    // Attempt to move both dimensions of the cursor. Each method contains bounds checking.
    move_cursor_x(x);
    move_cursor_y(y);
}

void ConsoleOutputHandler::move_cursor_x(uint16_t x)
{
    if(x > m_screen_character_width) return;

    m_cursor_position.first = x;
}

void ConsoleOutputHandler::move_cursor_y(uint16_t y)
{
    if(y > m_screen_character_height) return;

    m_cursor_position.first = y;
}

void ConsoleOutputHandler::modify_cursor_position(uint16_t x_modify, uint16_t y_modify)
{
    // If the modified x position is not in bounds.
    if(!_is_x_character_position_in_bounds(m_cursor_position.first + x_modify))
    {
        m_cursor_position.second += (m_cursor_position.first + x_modify + 1) % m_screen_character_width;
    }

    m_cursor_position.second += y_modify;

    m_cursor_position.first = (m_cursor_position.first + x_modify) % m_screen_character_width;
}

void ConsoleOutputHandler::add_ch(const char c, const std::string color)
{
    if(c == ' ')
    {
        ++m_cursor_position.first;

        // If the cursor's x position is not in bounds.
        if(!_is_x_character_position_in_bounds(m_cursor_position.first)) 
        { 
            // Place the cursor on the next line.
            add_new_line(); 
        }

        return;
    }

    else if(c == '\n')
    {
        add_new_line();
        return;
    }

    // This character is not a valid renderable charactere.
    else if(c < '!' || c > '~') return;

    m_text_ren_handler.add_ch(c, m_start_x + (m_cursor_position.first * m_font_scaled_width),
        m_start_y + (m_cursor_position.second * m_font_scaled_height * VERTICAL_SPACE_MODIFIER), 
        color);

    // Increment the cursor's x position.
    ++m_cursor_position.first;

    // If the cursor's x position is not in bounds.
    if(!_is_x_character_position_in_bounds(m_cursor_position.first)) 
    { 
        // Place the cursor on the next line.
        add_new_line(); 
    }
}

void ConsoleOutputHandler::add_str(const std::string str, const std::string color)
{
    for(const char c : str)
    {
        add_ch(c, color);
    }
}

void ConsoleOutputHandler::add_new_line(const uint8_t num)
{
    // Increment the cursor's y position by the requested amount.
    m_cursor_position.second += num;

    // Set the cursor's x position to the anchor.
    m_cursor_position.first = m_anchor;
}

void ConsoleOutputHandler::reset_cursor_position() { m_cursor_position = {0, 0}; }

void ConsoleOutputHandler::render() 
{ 
    m_text_ren_handler.render(); 
    reset_cursor_position();    
}



// Private

bool ConsoleOutputHandler::
    _is_x_character_position_in_bounds(const uint16_t character_x_position) const
{
    return character_x_position < m_screen_character_width;
}
