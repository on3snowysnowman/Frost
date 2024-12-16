/**
 * @file TextHandler.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class declaration.
 * @version 0.1
 * @date 24-12-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <SDL_ttf.h>
#include <SDL_image.h>

class TextHandler
{

public:

    TextHandler(SDL_Renderer* renderer, uint8_t font_size = 24);

private:

    // Members

    uint8_t m_font_size;

    SDL_Texture* font_atlas; // Full texture containing all renderable characters.
    SDL_Renderer* m_renderer;

    // Methods

    /** 
     * @brief Loads font from disk and creates a font atlas for quick character rendering. */
    void _init_font_atlas();
};
