#pragma once

#include <cstdint>
#include <unordered_set>
#include <queue>

#include "TextRenderingHandler.hpp"
#include "TextureHandler.hpp"
#include "DrawHandler.hpp"

/** 
 * @brief Handles placing characters on the screen in a structured way like a 
 * "Console" does.
 * 
 * Allows for printing text to the screen in a structured and bounds friendly 
 * way like you would expect a console to. Has an internal "cursor" that moves
 * across the screen automatically as text is added, and is placed on the next 
 * line when it reaches the edge of the screen. Uses the TextRenderingHandler 
 * as the underlying class to place individual characters on the screen.
 * 
 */
class ConsoleOutputHandler
{

/** 
 * I have thought about making ConsoleOutputHandler a child class of 
 * TextRenderingHandler to prevent the COH from needing to maintain many of the 
 * get and set methods that the TRH has. However, this ultimately proved to be 
 * not a great idea, as the public methods of the TRH like the font setting and 
 * changing of the sizes and what not, would not update the COH's tracking, 
 * such as the number of characters that can fit on the screen. I could make 
 * these sorts of methods virtual, then have COH overload the TRH ones that 
 * would cause conflicting issue but ultimately for efficiency and 
 * readability's sake, I've kept the classes independent. 
 * */

public:

    ConsoleOutputHandler();

    ConsoleOutputHandler(TextureHandler* texture_handler, 
        DrawHandler* draw_handler, uint8_t font_point_size, 
        std::string font_path);

    ConsoleOutputHandler(TextureHandler* texture_handler, 
        DrawHandler* draw_handler, uint8_t font_point_size, 
        std::string font_path, uint16_t start_x, uint16_t start_y, 
        uint16_t end_x, uint16_t end_y);

    ConsoleOutputHandler(const ConsoleOutputHandler& source);

    ConsoleOutputHandler(ConsoleOutputHandler&& source);

    ConsoleOutputHandler& operator=(const ConsoleOutputHandler& source);

    ConsoleOutputHandler& operator=(ConsoleOutputHandler&& source);

    /** 
     * @brief Resizes the dimensions of the COH, measured in pixels.
     * 
     * @param start_x Start x position.
     * @param start_y Start y position.
     * @param end_x Ending x position.
     * @param end_y Ending y position.
     * 
     */
    void resize_dimensions(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);


    /** 
     * @brief Moves the cursor to a new position on the screen, measured in characters. Contains bounds 
     * checking for the position. 
     * 
     * @param x X character position.
     * @param y Y character position.
     * 
     */
    void set_cursor(uint16_t x, uint16_t y);

    /** 
     * @brief Moves the cursor's x position to a new position on the screen, measured in characters. 
     * 
     * Contains bounds checking for the position.
     * 
     * @param x X character position.
     * 
     */
    void set_cursor_x(uint16_t x);

    /** 
     * @brief Moves the cursor's y position to a new position on the screen, measured in characters. 
     * 
     * Contains bounds checking for the position.
     * 
     * @param y Y character position.
     * 
     */
    void set_cursor_y(uint16_t y);

    /** 
     * @brief Modifies the cursor's position by the passed amount, measured 
     * in characters. 
     * 
     * This is an addition arithmetic. Contains bounds checking for the 
     * position, if the x boundary is breached, the cursor will be placed on 
     * the the next line and continue to be incremented. 
     * 
     * @param x_modify Amount to modify the cursor's x position by. 
     * @param y_modify Amount to modify the cursor's y position by.
     * 
     * */
    void modify_cursor_position(int16_t x_modify, int16_t y_modify);

    /** 
     * @brief Adds a character to the screen at the cursor's position.
     * 
     * @param c Character to add.
     * @param color Color of the character, default is White.
     * 
     */
    void add_ch(char c, std::string color = "White");

    void add_str(const char* chars, std::string color = "White");

    /** 
     * @brief Adds a string to the screen starting at the cursor's position. 
     * 
     * Supports automatic wrapping of characters if they breach the screen's bounds.
     * 
     * @param str String to add.
     * @param color Color of the string, default is White.
     * 
     */
    void add_str(std::string str, std::string color = "White");

    /** 
     * @brief Moves the cursor's y position down "num" times, and places the x position at the set
     * anchor. 
     * 
     * @param num Number of new lines to add, default is 1.
     * 
     */
    void add_new_line(uint8_t num = 1);
    
    /** 
     * @brief Resets the cursor's position to the top of the screen, with the
     * x position at the set anchor.
     * 
     * This is called automatically during each render call. 
     * 
     * */
    void reset_cursor_position();

    /** 
     * @brief Sets the anchor to a new value.
     * 
     * @param new_anchor New value.
     * 
     */
    void set_anchor(uint16_t new_anchor);

    /** 
     * @brief Sets the anchor to the current X position of the cursor.  
     * 
     * */
    void set_anchor_here();

    /** 
     * @brief Sets the focus to a new value.
     * 
     * @param new_focus New value.
     * 
     */
    void set_focus(uint16_t new_focus);

    /** 
     * @brief Renders the content buffered this frame, and resets the cursor's 
     * position to the top left. 
     * 
     * */
    void _render();

    /** 
     * @brief Sets the font point size to a new value. 11 is the minimum. 
     * 
     * */
    void set_font_size(uint8_t new_font_point_size);

    /** 
     * @brief Sets the font path to a new path, changing the font. 
     * 
     * */
    void set_font_path(std::string new_font_path);

    /** 
     * @brief Sets both the font path and size to new values. 
     * 
     * It is beneficial to use this method instead of the `set_font_size` and `set_font_path`
     * methods individually, as the font texture creation will only be called once in this method,
     * in contrast to being called twice with the two individual methods. 
     * 
     * */
    void set_font_path_and_size(uint8_t new_font_point_size, std::string new_font_path);

    /** 
     * @brief Returns the font point size. 
     * 
     * */
    uint8_t get_font_point_size() const;

    /** 
     * @brief Returns the font's width.
     * 
     * */
    uint8_t get_font_width() const;

    /** 
     * @brief Returns the font's height.
     * 
     * */
    uint8_t get_font_height() const;

    /** 
     * @brief Returns the focus. 
     * 
     * */
    uint16_t get_focus() const;

    /**
     * @brief Returns the anchor.
     * 
     */
    uint16_t get_anchor() const;

    /**
     * @brief Returns the cursor's position.
     * 
     */
    const std::pair<uint16_t, uint16_t>& get_cursor_position() const;
 
    /**
     * @brief Returns the current font path.
     * 
     */
    const std::string& get_current_font_path() const;

    /**
     * @brief Returns the available font paths.
     * 
     */
    const std::vector<std::string>& get_available_font_paths() const;

private:

    // Classes / Structs 

    /**
     * @brief Stores data for a character that has been queued to be rendered
     * on next render call.
     * 
     */
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

    // Width of the font.
    uint16_t m_font_width;

    // Height of the font.
    uint16_t m_font_height;

    // The x position in characters that a new line will be set to when placing a new line.
    uint16_t m_anchor {}; 

    // The y position in characters that the COH will center around, ensuring it is rendered.
    uint16_t m_focus {};

    /** The specific y position of the buffered rendered characters that the screen will start
     * at, based on the focus. */
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

    /** 
     * @brief Calculates the number of characters that can fit on the screen. 
     * 
     * */
    void _calculate_character_dimensions();

    /**
     * @brief 
     * 
     */
    void _calculate_view_around_focus();

    /** 
     * @brief Returns true if the passed x position, measured in characters, is 
     * within the bounds of the screen. 
     * 
     * */
    bool _is_x_character_position_in_bounds(uint16_t character_x_position) const;
};
