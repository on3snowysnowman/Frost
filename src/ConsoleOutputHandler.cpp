#include "ConsoleOutputHandler.hpp"
#include "Fr_Math.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif


// Constructors / Deconstructor

ConsoleOutputHandler::ConsoleOutputHandler() {}

ConsoleOutputHandler::ConsoleOutputHandler(TextureHandler* texture_handler) 
{ 
    m_text_ren_handler = TextRenderingHandler(texture_handler);
    // m_text_ren_handler.set_size_scale(2.0);

    m_font_scaled_width = m_text_ren_handler.get_scaled_font_width();
    m_font_scaled_height = m_text_ren_handler.get_scaled_font_height(); 
    resize_dimensions(0, 0, 0, 0);
}

ConsoleOutputHandler::ConsoleOutputHandler(TextureHandler* texture_handler, uint16_t start_x, 
    uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
    m_text_ren_handler = TextRenderingHandler(texture_handler);
    // m_text_ren_handler.set_size_scale(2.0);
    
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
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("ConsoleOutputHandler.resize_dimensions() -> Attempted to create"
            " with invalid dimensions. Setting dimensions to 0. ", Frost::WARN);
        #endif
        
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
        s_VERTICAL_SPACE_MODIFIER);

    m_end_character_render_y = m_screen_character_height;
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

    m_cursor_position.second = y;
}

void ConsoleOutputHandler::modify_cursor_position(int16_t x_modify, int16_t y_modify)
{
    // If the modified x position is not in bounds.
    if(!_is_x_character_position_in_bounds(m_cursor_position.first + x_modify))
    {
        m_cursor_position.second += (m_cursor_position.first + x_modify + 1) % m_screen_character_width;
    }

    m_cursor_position.second += y_modify;

    m_cursor_position.first = (m_cursor_position.first + x_modify) % m_screen_character_width;
}

void ConsoleOutputHandler::add_ch(char c, std::string color)
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

    // This character is not a valid renderable character.
    else if(c < '!' || c > '~') return;

    m_queued_characters.push(QueuedCharacter{
        c,
        m_cursor_position.first,
        m_cursor_position.second,
        color });

    // Increment the cursor's x position.
    ++m_cursor_position.first;

    // If the cursor's x position is not in bounds.
    if(!_is_x_character_position_in_bounds(m_cursor_position.first)) 
    { 
        // Place the cursor on the next line.
        add_new_line(); 
    }
}

void ConsoleOutputHandler::add_str(std::string str, std::string color)
{
    for(const char c : str)
    {
        add_ch(c, color);
    }
}

void ConsoleOutputHandler::add_new_line(uint8_t num)
{
    // Increment the cursor's y position by the requested amount.
    m_cursor_position.second += num;

    // Set the cursor's x position to the anchor.
    m_cursor_position.first = m_anchor;

    // Set the greatest y position to the cursor's y position if it is greater. 
    m_greatest_y_position_buffered = 
        Frost::return_largest_of_uint16s(m_greatest_y_position_buffered, m_cursor_position.second);
}

void ConsoleOutputHandler::clear_buffered_content() 
{ m_text_ren_handler.clear_buffered_content(); }

void ConsoleOutputHandler::reset_cursor_position() { m_cursor_position = {0, 0}; }

void ConsoleOutputHandler::set_anchor(uint16_t new_anchor) { m_anchor = new_anchor; }

void ConsoleOutputHandler::set_anchor_here() { m_anchor = m_cursor_position.first; }

void ConsoleOutputHandler::set_focus(uint16_t new_focus) 
{ 
    m_focus = new_focus; 

    // _calculate_view_around_focus();
}

void ConsoleOutputHandler::render() 
{  
    _calculate_view_around_focus();

    while(!m_queued_characters.empty())
    {
        const QueuedCharacter& character = m_queued_characters.front();

        // This character is outside the current viewing bounds.
        if(character.y_character_pos < m_start_character_render_y || 
            character.y_character_pos > m_end_character_render_y)
        {
            m_queued_characters.pop();
            continue;
        }

        // Draw the character to the screen.
        m_text_ren_handler.draw_character_now(character.symbol, 
            (m_start_x + (character.x_character_pos * m_font_scaled_width)), 
            m_start_y + ((character.y_character_pos - m_start_character_render_y) * 
            m_font_scaled_height * s_VERTICAL_SPACE_MODIFIER), 
            character.color);

        m_queued_characters.pop();
    }

    reset_cursor_position();    
    m_greatest_y_position_buffered = 0;
}

uint16_t ConsoleOutputHandler::get_focus() const { return m_focus; }

uint16_t ConsoleOutputHandler::get_anchor() const { return m_anchor; }

const std::pair<uint16_t, uint16_t>& ConsoleOutputHandler::get_cursor_position() const
{ return m_cursor_position; }


// Private

void ConsoleOutputHandler::_calculate_view_around_focus()
{
    uint16_t half_screen_y_position = std::floor(m_screen_character_height * 0.5f);

    // If the focus or greatest y position buffered is within the render of half the screen bounds.
    if(m_focus < half_screen_y_position) 
    { 
        m_start_character_render_y = 0;
        m_end_character_render_y = m_screen_character_height;
        return;
    }

    // If the distance between the greatest y position and the focus is greater than the half the 
    // screen height.
    else if (m_greatest_y_position_buffered - m_focus > half_screen_y_position)
    {
        m_start_character_render_y = m_focus - half_screen_y_position;
        m_end_character_render_y = m_focus + (m_screen_character_height - half_screen_y_position);
        return;
    }

    // The distance between the greatest y position and the focus is less than half the screen 
    // height, so set the start y position of the render as the the last 
    // "m_screen_character_height" characters of the queued_characters.

    m_start_character_render_y = m_greatest_y_position_buffered - m_screen_character_height;
    m_end_character_render_y = m_greatest_y_position_buffered;
}

bool ConsoleOutputHandler::
    _is_x_character_position_in_bounds(uint16_t character_x_position) const
{
    return character_x_position < m_screen_character_width;
}
