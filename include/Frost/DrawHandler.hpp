/**
 * @file Renderer.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Contains the declaration of the Renderer class.
 * @version 0.1
 * @date 02-04-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <SDL_image.h>


class DrawHandler
{

public:

    DrawHandler();

    DrawHandler(SDL_Renderer* renderer, 
        const std::string& colors_path, uint16_t screen_width, 
        uint16_t screen_height);

    /** 
     * @brief Draws a portion of the Texture to the buffer, rendered 
     * to the screen later per render call.
     * 
     * The portion of the texture is determined by `src`, while the dimensions 
     * on the screen are determined by `dest`.
     * const.
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

    /** 
     * @brief Returns a const reference to the tracked colors. 
     * 
     * The key of each pair is the name of the color. 
    */
    const std::unordered_map<std::string, SDL_Color>& get_colors() const;
    
private:

    // Members

    // Path to the color data file.
    std::string m_color_data_path;

    /** Color names to their SDL_Color objects. */
    std::unordered_map<std::string, SDL_Color> m_colors;

    SDL_Renderer* m_renderer;


    // Methods

    /** Creates and registers SDL_Color objects from the color data file. */
    void _fetch_colors_from_disk();
};
