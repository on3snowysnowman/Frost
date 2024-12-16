/**
 * @file TextHandler.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 24-12-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "TextHandler.hpp"


// Constructors / Deconstructor

TextHandler::TextHandler(SDL_Renderer* renderer, uint8_t font_size)
{
    m_renderer = renderer;
    m_font_size = font_size;

    _init_font_atlas();
}


// Public



// Private

void TextHandler::_init_font_atlas()
{
    const uint16_t ATLAS_WIDTH = 1024;
    const uint16_t ATLAS_HEIGHT = 1024;

    font_atlas = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        )
}

