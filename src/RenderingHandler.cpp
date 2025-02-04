/**
 * @file RenderingHandler.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Contains the implementation of the RenderingHandler class.
 * @version 0.1
 * @date 2025-02-03
 *
 * @copyright Copyright (c) 2024 Joel Height
 */

#include <stdexcept>

#include "RenderingHandler.hpp"
#include "CrashOutputHandler.hpp"
#include "JsonHandler.hpp"

// Constructors / Deconstructor

RenderingHandler::RenderingHandler() { m_renderer = nullptr; }

RenderingHandler::RenderingHandler(SDL_Renderer* renderer, 
    std::string colors_path, uint16_t screen_width, uint16_t screen_height) 
{
    m_renderer = renderer;

    m_color_data_path = colors_path;

    _init_texture_buffer(screen_width, screen_height);

    _fetch_colors_from_disk();
}


// Public

void RenderingHandler::_render_buffer_to_screen() const
{
    // Reset render target to the screen.
    SDL_SetRenderTarget(m_renderer, nullptr);

    // Copy the buffer onto the screen.
    SDL_RenderCopy(m_renderer, m_buffer_texture, &m_buffer_dimensions, 
        &m_buffer_dimensions);

    // Reset render target back to buffer.
    SDL_SetRenderTarget(m_renderer, m_buffer_texture);
}

void RenderingHandler::draw(SDL_Texture* texture, const SDL_Rect& source, 
    const SDL_Rect& dest)
{
    SDL_SetRenderTarget(m_renderer, m_buffer_texture);

    // Copy the texture into the buffer at the specified dimensions.
    SDL_RenderCopy(m_renderer, texture, &source, &dest);
}

void RenderingHandler::draw(SDL_Texture* texture, const SDL_Rect& source, 
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
    
    // Store the original color values of the texture, since the texture's color channels must be
    // modified during the rendering process, and needs to be restored after to their original value.

    SDL_Color original_texture_color;

    SDL_GetTextureColorMod(texture, &original_texture_color.r, &original_texture_color.g, 
        &original_texture_color.b);

    // Update the texture's color mod.
    SDL_SetTextureColorMod(texture, targ_color->r, targ_color->g, targ_color->b);

    // Copy the texture into the buffer.
    SDL_RenderCopy(m_renderer, texture, &source, &dest);

    // Restore the texture to its original color.
    SDL_SetTextureColorMod(texture, original_texture_color.r, original_texture_color.g,
        original_texture_color.b);
}

const std::unordered_map<std::string, SDL_Color>& 
    RenderingHandler::get_colors() const { return m_colors; }

// Private

void RenderingHandler::_fetch_colors_from_disk()
{
    // List of colors in json format.
    const json colors = JsonHandler::get(m_color_data_path);

    // Iterate through each color.
    for(const json& color : colors)
    {
        // Create a color, and register it in the map using its name as the key.
        m_colors[color.at(0)] = SDL_Color {color.at(1), color.at(2), color.at(3)};
    }
}

void RenderingHandler::_init_texture_buffer(uint16_t screen_width, uint16_t screen_height)
{
    // Create the buffer texture with the dimensions of the screen.
    m_buffer_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, screen_width, screen_height);

    // Set the initial render target as the buffer texture, so that when draw
    // calls start flooding in, they are applied to the buffer.
    SDL_SetRenderTarget(m_renderer, m_buffer_texture);

    // Set the blend mode to blended, so that when the buffer is drawn onto the
    // screen, alpha values are respected.
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    m_buffer_dimensions.x = 0;
    m_buffer_dimensions.y = 0;
    m_buffer_dimensions.w = screen_width;
    m_buffer_dimensions.h = screen_height;
}

