/**
 * @file DrawHandler.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 02-04-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdexcept>

#include "DrawHandler.hpp"
#include "CrashOutputHandler.hpp"
#include "JsonHandler.hpp"


// Constructors / Deconstructor

DrawHandler::DrawHandler() 
{
    m_renderer = nullptr;
}

DrawHandler::DrawHandler(SDL_Renderer *renderer,
     const std::string& colors_path, uint16_t screen_width, 
     uint16_t screen_height)
{
    m_renderer = renderer;

    m_color_data_path = colors_path;
    _fetch_colors_from_disk();
}


// Public

void DrawHandler::draw(SDL_Texture* texture, const SDL_Rect& source, 
    const SDL_Rect& dest)
{
    // Copy the texture into the buffer at the specified dimensions.
    SDL_RenderCopy(m_renderer, texture, &source, &dest);
}

void DrawHandler::draw(SDL_Texture* texture, const SDL_Rect& source, 
    const SDL_Rect& dest, const std::string& color_name)
{
    SDL_Color* targ_color;

    try
    {
        targ_color = &m_colors.at(color_name);
    }

    // Color name doesn't exist.
    catch(const std::out_of_range& e)
    {
        OUTPUT_CRASH_DETAILS(" where 'color_name' = '" + color_name + 
        "' : Unknown color name.\n");
        exit(1);
    }
    
    // Store the original color values of the texture, since the texture's 
    // color channels must be modified during the rendering process, and needs 
    // to be restored after to their original value.

    SDL_Color original_texture_color;

    SDL_GetTextureColorMod(texture, &original_texture_color.r, &original_texture_color.g, 
        &original_texture_color.b);

    // Update the texture's color mod.
    SDL_SetTextureColorMod(texture, targ_color->r, targ_color->g, targ_color->b);

    // Copy the texture into the render target.
    SDL_RenderCopy(m_renderer, texture, &source, &dest);

    // Restore the texture to its original color.
    SDL_SetTextureColorMod(texture, original_texture_color.r, original_texture_color.g,
        original_texture_color.b);
}

const std::unordered_map<std::string, SDL_Color>& DrawHandler::get_colors() const
{ return m_colors; }


// Private

void DrawHandler::_fetch_colors_from_disk()
{
    // List of colors in json format.
    const json colors = JsonHandler::get(m_color_data_path);

    // Iterate through each color.
    for(const json& color : colors)
    {
        // Create a color, and register it in the map using its name as the key.
        m_colors[color.at(0)] = 
            SDL_Color {color.at(1), color.at(2), color.at(3), 255};
    }
}