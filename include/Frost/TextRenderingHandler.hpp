#pragma once

#include <cstdint>
#include <string>
#include <queue>
#include <unordered_map>

#include <SDL2/SDL_image.h>

#include "TextureHandler.hpp"

/** Underlying class that handles the rendering of individual characters to the screen. Can be used 
 * on its own, but is typically built on top of by other classes with more convenient methods for 
 * rendering structured text, such as the ConsoleOutputHandler. This contains the ability to render 
 * text at a scaled resolution. When characters are added, they are pushed onto a queue. This means
 * that each render call, the queue is emptied and a manual "clear screen call" is not required. 
 * A clear function does exist, but only use it if the intent is to clear the added content this 
 * frame before it has a chance to render.
*/
class TextRenderingHandler
{

public:

    TextRenderingHandler();

    TextRenderingHandler(TextureHandler* texture_handler);

    /** Renders buffered content this frame to the screen. */
    void render();

    /** Clears the buffered content this frame. Only use this function to clear the content added 
     * this frame before it has a chance to render. Do not use this method as a scheduled clear 
     * screen after each frame.
     */
    void clear_buffered_content();

    /** Increments the font size scaling factor by the passed amount. This value is clamped to a
     * maximum of 5.0.
     * 
     * @param amount Amount to increase.
     */
    void increase_size_scale(float amount);

    /** Sets the font size scaling factor to the passed amount. This value is clamped to a range
     * of 1.0 to 5.0.
     * 
     * @param Amount Amount to set the factor to.
     */
    void set_size_scale(float amount);

    /** Decrements the font size scaling factor by the passed amount. This value is clamped to a 
     * minimum of 1.0
     * 
     * @param amount Amount to decrease.
    */
    void decrease_size_scale(float amount);

    /** Draws a character to the screen immediately, skipping the buffering process to wait for a 
     * render call. Only use this method over the add_ch method if it is known that this method is
     * called between the clear_SDL_renderer() and present_SDL_renderer() method in the engine, 
     * otherwise the passed character will be erased from the screen before it has a chance to 
     * render.
     */
    void draw_character_now(char c, uint16_t x, uint16_t y, std::string color = "White");

    /** Adds a char to the screen at a position with a color. 
     * 
     * @param c Character to add.
     * @param x X position in pixels.
     * @param y Y position in pixels.
     * @param color Color of the character.
    */
    void add_ch(char c, uint16_t x, uint16_t y, std::string color = "White");

    /** Returns the font size scaling factor that each character is scaled by. */
    float get_size_scale() const;

    /** Returns the font width scaled by the font size scaling factor. */
    uint16_t get_scaled_font_width() const;

    /** Returns the font height scaled by the font size scaling factor. */
    uint16_t get_scaled_font_height() const;

private:

    // Classes / Structs

    // Stores data for a character that has been queued to be rendered on next render call.
    struct QueuedCharacter
    {
        // Character symbol this struct represents
        char symbol {};

        // Source dimensions to portion the font texture.
        SDL_Rect source;

        // Dimensions to render to the screen. 
        SDL_Rect dest;

        // Color of the character.
        std::string color = "White";
    };


    // Members

    // Path to the active font's data file. 
    std::string m_font_data_path = "data/Frost_font_data.json"; 

    uint8_t m_font_width; // Width of each character in the active font in pixels.
    uint8_t m_font_height; // Height of each character in the active font in pixels.

    float m_size_scale_factor = 1.0f; // The factor to scale each rendered character's size by.

    // Maximum value the size scale factor can be increased to.
    static constexpr const float MAXIMUM_SIZE_SCALE_FACTOR = 5.0f;

    // Characters that are queued to be rendered on next render call.
    std::queue<QueuedCharacter> m_queued_characters;

    // Character symbols to their source positions inside the font texture.
    std::unordered_map<char, std::pair<uint8_t, uint8_t>> m_char_source_positions;

    SDL_Texture* m_font_texture; // Full texture of the font, contains all the characters.

    TextureHandler* m_texture_handler;


    // Methods

    /** Fetches the font data from the disk at the active font file and creates the necessary
     * components and data needed for rendering characters.*/
    void _load_font_from_disk();

    /** Returns the passed value scaled by the font size scaling factor. 
     * 
     * @param value Value to scale. 
    */
    uint16_t _get_scaled_value(uint16_t value) const;
};
