#pragma once

#include <cstdint>
#include <unordered_set>

#include "TextRenderingHandler.h"
#include "TextureHandler.h"

/** Allows for printing text to the screen in a structured and bounds friendly way like you would 
 * expect a console to. Has an internal "cursor" that moves across the screen automatically as 
 * text is added, and is placed on the next line when it reaches the edge of the screen. Uses the 
 * TextRenderingHandler as the underlying class to place individual characters on the screen.
 */
class ConsoleOutputHandler
{

public:

    ConsoleOutputHandler();

    ConsoleOutputHandler(TextureHandler* texture_handler);

    ConsoleOutputHandler(TextureHandler* texture_handler, uint16_t start_x, uint16_t start_y, 
        uint16_t end_x, uint16_t end_y);

    /** Resizes the dimensions of the COH, measured in pixels.
     * 
     * @param start_x Start x position.
     * @param start_y Start y position.
     * @param end_x Ending x position.
     * @param end_y Ending y position.
     */
    void resize_dimensions(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);

    /** Moves the cursor to a new position on the screen, measured in characters. Contains bounds 
     * checking for the position. 
     * 
     * @param x X character position.
     * @param y Y character position
     */
    void move_cursor(const uint16_t x, const uint16_t y);

    /** Moves the cursor's x position to a new position on the screen, measured in characters. 
     * Contains bounds checking for the position.
     * 
     * @param x X character position.
     */
    void move_cursor_x(const uint16_t x);

    /** Moves the cursor's y position to a new position on the screen, measured in characters. 
     * Contains bounds checking for the position.
     * 
     * @param y Y character position.
     */
    void move_cursor_y(const uint16_t y);

    /** Modifies the cursor's position by the passed amount, measured in characters. This is an 
     * addition arithmetic. Contains bounds checking for the position, if the x boundary is 
     * breached, the cursor will be placed on the the next line and continue to be incremented. 
     * 
     * @param x_modify Amount to modify the cursor's x position by. 
     * @param y_modify Amount to modify the cursor's y position by.
     * */
    void modify_cursor_position(const uint16_t x_modify, const uint16_t y_modify);

    void add_ch(char c, const std::string color = "White");

    void add_str(const std::string str, const std::string color = "White");

    void add_new_line(const uint8_t num = 1);

    void clear_buffered_content();

    void reset_cursor_position();

    void set_anchor();

    void set_focus();

    void render();

    const std::pair<uint16_t, uint16_t> get_cursor_position() const;

private:

    // Members

    // Starting X position of the screen.
    uint16_t m_start_x;

    // Starting Y position of the screen.
    uint16_t m_start_y;

    // Ending X position of the screen.
    uint16_t m_end_x;

    // Ending Y position of the screen.
    uint16_t m_end_y;

    // Width of the screen, measured in characters rounded down.
    uint16_t m_screen_character_width;

    // Height of the screen, measure in characters rounded down.
    uint16_t m_screen_character_height;

    // Width of the font, scaled with the TextRenderingHandler's font size multiplier.
    uint16_t m_font_scaled_width;

    // Height of the font, scaled with the TextRenderingHandler's font size multiplier
    uint16_t m_font_scaled_height;

    // The x position that a new line will "anchor" to (be set to) when placing a new line.
    uint16_t m_anchor {}; 

    /** The scale factor that the space in between the characters along the vertical scale will be
     * multiplied by. */
    static constexpr const float VERTICAL_SPACE_MODIFIER = 1.5f;

    /** Cursor's position from the start of the COH dimensions. Measured in characters. */
    std::pair<uint16_t, uint16_t> m_cursor_position {0, 0};

    /** Each COH has its own TextRenderingHandler, since if they all shared the same one, the 
     * Engine would be limited to rendering only a single font and single font size at a time.
     */
    TextRenderingHandler m_text_ren_handler;


    // Methods

    /** Returns true if the passed x position, measured in characters, is within the bounds of the
     * screen. */
    bool _is_x_character_position_in_bounds(const uint16_t character_x_position) const;
};
