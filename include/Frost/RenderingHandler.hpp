/**
 * @file RenderingHandler.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Contains the declaration of the RenderingHandler class.
 * @version 0.1
 * @date 2025-02-03
 *
 * @copyright Copyright (c) 2024 Joel Height
 */

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <SDL_image.h>


/** 
 * @brief Handles drawing and presenting SDL_Textures to the screen each frame.
 * 
 * Uses an internal texture buffer for efficient pixel modification through
 * draw calls.
 */
class RenderingHandler
{

public:

    RenderingHandler();

    RenderingHandler(SDL_Renderer* renderer, std::string colors_path, 
        uint16_t screen_width, uint16_t screen_height);

    /** 
     * @brief Renders buffered textures to the screen. 
     * 
     * @attention This method is called by the Engine during the 'render' 
     * portion of its update loop. The user should not call this method.
     */
    void _render_buffer_to_screen() const;

    /** 
     * @brief Draws a portion of the Texture to the buffer, rendered 
     * to the screen later per render call.
     * 
     * The portion of the texture is determined by `src`, while the dimensions 
     * on the screen are determined by `dest`.
     * 
     * @param texture Texture to draw.
     * @param source Dimensions to portion from the texture.
     * @param dest Dimensions to place on the screen.
     */
    void draw(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& dest);

    /** 
     * @brief Draws a portion of the Texture with a color applied to the 
     * buffer, rendered to the screen later per render call.
     * 
     * The portion of the texture is determined by `src`, while the dimensions 
     * on the screen are determined by `dest`. The `color` is applied to the 
     * texture.
     * 
     * @param texture Texture to draw.
     * @param source Dimensions to portion from the texture.
     * @param dest Dimensions to place on the screen.
     * @param color_name Name of the color to apply.
     */
    void draw(SDL_Texture* texture, const SDL_Rect& source, 
        const SDL_Rect& dest, const std::string& color_name);

    /** Returns a const reference to the tracked colors. The key of each pair
     * is the name of the color. 
    */
    const std::unordered_map<std::string, SDL_Color>& get_colors() const;

private:

    // Members

    /** Dimensions of the buffer texture. */
    SDL_Rect m_buffer_dimensions;

    // Path to the color data file.
    std::string m_color_data_path;

    /** Color names to their SDL_Color objects. */
    std::unordered_map<std::string, SDL_Color> m_colors;

    SDL_Renderer* m_renderer;

    /** Buffer for rendering textures to the screen. This is used so that the 
     * GPU can efficiently modify this texture through individual `draw()` 
     * calls each frame, and only a single draw call to the screen needs to be
     * made inside the `render` method. */
    SDL_Texture* m_buffer_texture;


   // Methods

    /** Creates and registers SDL_Color objects from the color data file. */
    void _fetch_colors_from_disk();

    /** 
     * @brief Creates and initializes the texture buffer and its dimensions, 
     * dependant on passed screen dimensions.
     */
   void _init_texture_buffer(uint16_t screen_width, uint16_t screen_height); 
};
