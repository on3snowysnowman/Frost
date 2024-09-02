#pragma once

#include <cstdint>

#include <SDL2/SDL_image.h>


typedef uint64_t sprite_id;

/** @brief Pure storage struct. Contains data for rendering a portion of an SDL_Texture to the 
 * screen. 
 * */
struct Sprite
{
    Sprite(sprite_id _id) : id(_id) {}

    // If the Sprite is currently rendering.
    bool is_rendering {};

    // Contains the dimensions for splicing the SDL_Texture.
    SDL_Rect splice_dimensions;

    // Contains the dimensions for placing the Sprite on the screen.
    SDL_Rect display_dimensions;

    const sprite_id id;  // Unique ID of this Sprite
    
    SDL_Texture* texture {}; // Texture that the Sprite will splice from.
};
