#pragma once

#include <cstdint>
#include <unordered_set>
#include <queue>

#include "TextRenderingHandler.hpp"
#include "TextureHandler.hpp"

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
    void move_cursor(uint16_t x, uint16_t y);

    /** Moves the cursor's x position to a new position on the screen, measured in characters. 
     * Contains bounds checking for the position.
     * 
     * @param x X character position.
     */
    void move_cursor_x(uint16_t x);

    /** Moves the cursor's y position to a new position on the screen, measured in characters. 
     * Contains bounds checking for the position.
     * 
     * @param y Y character position.
     */
    void move_cursor_y(uint16_t y);

    /** Modifies the cursor's position by the passed amount, measured in characters. This is an 
     * addition arithmetic. Contains bounds checking for the position, if the x boundary is 
     * breached, the cursor will be placed on the the next line and continue to be incremented. 
     * 
     * @param x_modify Amount to modify the cursor's x position by. 
     * @param y_modify Amount to modify the cursor's y position by.
     * */
    void modify_cursor_position(int16_t x_modify, int16_t y_modify);

    /** Adds a character to the screen at the cursor's position.
     * 
     * @param C Character to add.
     * @param color Color of the character, default is White.
     */
    void add_ch(char c, std::string color = "White");

    /** Adds a string to the screen starting at the cursor's position. Supports automatic wrapping
     * of characters if they breach the screen's bounds.
     * 
     * @param str String to add.
     * @param color Color of the string, default is White.
     */
    void add_str(std::string str, std::string color = "White");

    /** Moves the cursor's y position down "num" times, and places the x position at the set
     * anchor. 
     * 
     * @param num Number of new lines to add, default is 1.
     */
    void add_new_line(uint8_t num = 1);

    /** Clears the buffered content this frame. Only use this function to clear the content added 
     * this frame before it has a chance to render. Do not use this method as a scheduled clear 
     * screen after each frame.
     */
    void clear_buffered_content();

    /** Resets the cursor's position to the top left. This is called automatically during each 
     * render call. */
    void reset_cursor_position();

    /** Sets the anchor to a new value.
     * 
     * @param new_anchor New value.
     */
    void set_anchor(uint16_t new_anchor);

    /** Sets the anchor to the current X position of the cursor.  */
    void set_anchor_here();

    /** Sets the focus to a new value.
     * 
     * @param new_focus New value.
     */
    void set_focus(uint16_t new_focus);

    /** Renders the content buffered this frame, and resets the cursor's position to the top left. */
    void render();

    /** Returns the focus. */
    uint16_t get_focus() const;

    /** Returns the anchor. */
    uint16_t get_anchor() const;

    // Returns a const reference to the cursor's position.
    const std::pair<uint16_t, uint16_t>& get_cursor_position() const;

private:

    // Classes / Structs 

    // Stores data for a character that has been queued to be rendered on next render call.
    struct QueuedCharacter
    {
        // Character symbol this struct represents
        char symbol {};

        uint16_t x_character_pos {};

        uint16_t y_character_pos {};

        // Color of the character.
        std::string color = "White";
    };


    // Members

    // Starting X position of the screen in pixels.
    uint16_t m_start_x;

    // Starting Y position of the screen in pixels.
    uint16_t m_start_y;

    // Ending X position of the screen in pixels.
    uint16_t m_end_x;

    // Ending Y position of the screen pixels.
    uint16_t m_end_y;

    // Width of the screen, measured in characters rounded down.
    uint16_t m_screen_character_width;

    // Height of the screen, measure in characters rounded down.
    uint16_t m_screen_character_height;

    // Width of the font, scaled with the TextRenderingHandler's font size multiplier.
    uint16_t m_font_scaled_width;

    // Height of the font, scaled with the TextRenderingHandler's font size multiplier
    uint16_t m_font_scaled_height;

    // The x position in characters that a new line will be set to when placing a new line.
    uint16_t m_anchor {}; 

    // The y position in characters that the COH will center around, ensuring it is rendered.
    uint16_t m_focus {};

    /** The specific y position of the buffered rendered characters that the screen will start
     * at, based on the focus.
     */
    uint16_t m_start_character_render_y {0};

    uint16_t m_end_character_render_y {0};

    // The greatest y position that a character has been queued to render this frame.
    uint16_t m_greatest_y_position_buffered {0};

    /** The scale factor that the space in between the characters along the vertical scale will be
     * multiplied by. */
    static constexpr const float s_VERTICAL_SPACE_MODIFIER = 1.0f;

    /** Cursor's position from the start of the COH dimensions. Measured in characters. */
    std::pair<uint16_t, uint16_t> m_cursor_position {0, 0};

    // Characters queued during this frame that will be rendered on the next render call.
    std::queue<QueuedCharacter> m_queued_characters;

    /** Each COH has its own TextRenderingHandler, since if they all shared the same one, the 
     * Engine would be limited to rendering only a single font and single font size at a time.
     */
    TextRenderingHandler m_text_ren_handler;


    // Methods


    void _calculate_view_around_focus();

    /** Returns true if the passed x position, measured in characters, is within the bounds of the
     * screen. */
    bool _is_x_character_position_in_bounds(uint16_t character_x_position) const;
};
