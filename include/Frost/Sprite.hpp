#pragma once

#include <SDL_image.h>

/** @brief Pure storage struct. Contains data for rendering a portion of an SDL_Texture to the 
 * screen. 
 * */
struct Sprite
{
    /**
     * Pixel dimensions to define the region of the texture to splice for 
     * this Sprite.
     */
    SDL_Rect splice_dimensions;
            
    /**
     * Texture that this Sprite will splice from.
     * This must be a valid SDL_Texture pointer.
     */
    SDL_Texture* texture = nullptr;

};
