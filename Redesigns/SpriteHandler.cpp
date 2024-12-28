/**
 * @file SpriteHandler.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.2
 * @date 12-28-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <algorithm>

#include "SpriteHandler.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif


// Constructors / Deconstructor

SpriteHandler::SpriteHandler() 
{
    // Default constructor should only be called for a placeholder object.

    m_texture_handler = nullptr;
}

SpriteHandler::SpriteHandler(TextureHandler* texture_handler)
{
    m_texture_handler = texture_handler;
}


// Public

void SpriteHandler::render()
{   
    // Iterate through each active z layer.
    for(const uint16_t layer : m_active_layers)
    {
        // Iterate through the sprites on this layer.
        for(const sprite_id id : m_layers_to_sprites.at(layer))
        {
            Sprite& sprite = s_all_sprites.at(id);

            // Calculate the size of the Sprite displayed on screen using the size of the splice
            // dimensions upscaled by the scale factor.
            sprite.display_dimensions.w = sprite.splice_dimensions.w * m_sprite_scale_factor;
            sprite.display_dimensions.h = sprite.splice_dimensions.h * m_sprite_scale_factor;

            m_texture_handler->draw(sprite.texture, sprite.splice_dimensions, 
                sprite.display_dimensions);
        }
    }
}

void SpriteHandler::set_sprite_position(sprite_id id, uint16_t x, uint16_t y)
{
    if(!_is_id_valid(id))
    {
        // Crash the program, since no valid Sprite object exists for this ID.

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("SpriteHandler.set_sprite_position() -> Sprite ID : \"" + 
            std::to_string(id) + "\" does not exist.", Frost::ERR);
        #endif

        exit(1);
    }

    Sprite& sprite = s_all_sprites.at(id);

    // Update the Sprite's position.
    sprite.display_dimensions.x = x;
    sprite.display_dimensions.y = y;
}

void SpriteHandler::flag_render(sprite_id id, uint16_t layer)
{
    // Don't allow duplicate rendering of a single Sprite.
    if(_is_sprite_rendering(id)) return;

    // Find the position in the sorted vector where this layer would be at if it exists or where it
    // should be inserted if it does not. 
    const std::vector<uint16_t>::const_iterator it = 
        std::lower_bound(m_active_layers.begin(), m_active_layers.end(), layer);

    // This layer does not exist yet.
    if(*it != layer)
    {
        m_active_layers.insert(it, layer);
        m_layers_to_sprites.emplace(layer, std::vector<sprite_id>{});
    }

    m_layers_to_sprites.at(layer).push_back(id);
}


// Private

void SpriteHandler::_insert_id_in_layer_vector(sprite_id id, uint16_t layer)
{
    std::vector<sprite_id>& layer_vector = m_layers_to_sprites.at(layer);

    // Find the position in the sorted layer vector where this id should be inserted.
    const std::vector<sprite_id>::const_iterator it = 
        std::lower_bound(layer_vector.begin(), layer_vector.end(), id);

    layer_vector.insert(it, )
}
