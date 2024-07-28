#pragma once

#include <cstdint>
#include <string>

#include "ConsoleOutputHandler.hpp"
#include "SpriteHandler.hpp"
#include "TextureHandler.hpp"

/**
 * @brief Offers ease of access for rendering characters and sprites to the screen without
 * meddling with the underlying components on their own.
 * 
 * Under typical scenarios, the user will not need to create a Window instance manually, each 
 * Frost::Menu they create will automatically construct a Window instance.
 * 
 * This class does not offer much unique funcitonality that can't be obtained through the 
 * underlying classes (ConsoleOutputHandler and SpriteHandler), rather it combines the 
 * functionality of the two in a way that makes it easy to render things to the screen. For
 * Sprites to be rendered, the support_sprites bool needs to be set as true in the constructor.
 * This bool is simply here to potentially save on memory: If the user knows they will not need to 
 * render Sprites to this window, keeping this bool as false means that a SpriteHandler will not be
 * created for this Window. Do not attempt to render Sprites if you have flagged this Window to not
 * support Sprites.
 * 
 * Along with combining the functionality of the two underlying rendering classes, the Window 
 * renders a padding zone that acts as a nice border between the edges of the application window 
 * and other Windows created next to it.
 * 
 * If a Window instance is used as a standalone, the render method will need to be called manually.
 * However, Frost supports automatic tracking of active Windows that needs to be updated through 
 * the WindowManger. Users will not need to worry about tracking active Windows that need to be 
 * rendered, as this is done through the MenuManager's Menu tracking process. When a Menu is set to
 * active, the MenuManager flags the WindowManager that that Menu is active, and its respective 
 * Window needs to be rendered. All the user needs to do is to flag the MenuManager class to 
 * activate a Menu, and it will automatically render its Window.
 */
class Window
{

public:

    Window();

    /** Window Ctor.
     * 
     * @param tex_handler Instance of the TextureHandler
     * @param start_x Starting x position of the window in pixels.
     * @param start_y Starting y position of the window in pixels.
     * @param end_x Ending x position of the window in pixels.
     * @param end_y // Ending y position of the window in pixels.
     * @param support_sprites If the Window should support Sprite rendering.
     */
    Window(TextureHandler* tex_handler, uint16_t start_x, uint16_t start_y, uint16_t end_x, 
        uint16_t end_y, bool support_sprites = false);

    /** Clears buffered content before it has a chance to be rendered this frame. Only call this 
     * method if the intention is to clear buffered content this frame before it has a chance to 
     * render, not to perform a per frame "clear screen" call. By design content added each frame 
     * is automatically cleared after each render since they are added on to a queue and popped 
     * off.
     */
    void clear();

    /** Renders buffered characters to the screen, along with signalling the SpriteHandler to 
     * render its flagged Sprites. */
    void render();

    void add_ch(char c);

    void add_ch(char c, std::string color);

    void add_str(std::string str);

    void add_str(std::string str, std::string color);

    void add_new_line(uint8_t num = 1);

    void set_sprite_position(uint16_t x, uint16_t y);

    void flag_sprite_render(const sprite_id id);

    void flag_sprite_de_render(const sprite_id id);

    void delete_sprite(const sprite_id id);

    void set_sprite_scale_factor(float new_scale_factor);

    



private:

    // Members

    /** Width of the visual border that will be rendered around the window, and characters will
     * be placed inside.
     */
    const uint8_t BORDER_SIZE = 10; 

    uint16_t m_start_x; // Starting x position of the window in pixels.
    uint16_t m_start_y; // Starting y position of the window in pixels.
    uint16_t m_end_x; // Ending x position of the window in pixels.
    uint16_t m_end_y; // Ending y position of the window in pixels.

    ConsoleOutputHandler* m_coh;
    SpriteHandler* m_sp_handler;


    // Methods

    void _resize_window(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
};
