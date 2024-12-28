/**
 * @file SpriteHandler.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class implementation.
 * @version 0.2
 * @date ?
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <algorithm>

#include "SpriteHandler.hpp"
#include "Fr_Math.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif



// Static Members

std::vector<Sprite> SpriteHandler::s_all_sprites;

std::unordered_set<sprite_id> SpriteHandler::s_available_ids;

std::unordered_map<SDL_Texture*, uint64_t> SpriteHandler::s_texture_dependencies;


// Constructors / Deconstructor

SpriteHandler::SpriteHandler() 
{
    m_texture_handler = nullptr;
}

SpriteHandler::SpriteHandler(TextureHandler* texture_handler)
{
    m_texture_handler = texture_handler;
}


// Public

void SpriteHandler::render()
{
    for(const sprite_id& id : m_sprites_to_render)
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

    Sprite& targ_sprite = s_all_sprites.at(id);

    // Update the Sprite's position.
    targ_sprite.display_dimensions.x = x;
    targ_sprite.display_dimensions.y = y;
}

void SpriteHandler::flag_render(sprite_id id, uint16_t layer)
{ 
    if(!_is_id_valid(id)) 
    {
        #ifdef FROST_DEBUG

        // Crash the program, since no valid Sprite object exists for this ID.
        ProgramOutputHandler::log("SpriteHandler.flag_render() -> Sprite ID : \"" + 
            std::to_string(id) + "\" does not exist.", Frost::ERR);
        #endif

        exit(1);
    }

    Sprite& targ_sprite = s_all_sprites.at(id);

    // If this Sprite is already rendering.
    if(targ_sprite.is_rendering) return;

    targ_sprite.is_rendering = true;

    _place_id_in_rendering_ids(id);
}

void SpriteHandler::deflag_render(sprite_id id)
{ 
    if(!_is_id_valid(id))
    {
        // Crash the program, since no valid Sprite object exists for this ID.
        
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("SpriteHandler.deflag_render() -> Sprite ID : \"" + 
            std::to_string(id) + "\" does not exist.", Frost::ERR);
        #endif
        
        exit(1);
    }

    Sprite& targ_sprite = s_all_sprites.at(id);

    // If this Sprite is not rendering.
    if(!targ_sprite.is_rendering) return;

    targ_sprite.is_rendering = false;

    _remove_id_from_rendering_ids(id);
}

void SpriteHandler::delete_sprite(sprite_id id)
{
    if(!_is_id_valid(id))
    {
        // Crash the program, since no valid Sprite object exists for this ID.

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("SpriteHandler.delete_sprite() -> Sprite ID : \"" + 
            std::to_string(id) + "\" does not exist.", Frost::ERR);
        #endif

        exit(1);
    }

    // "Delete" this Sprite by making its ID available, which in turn makes it unavailable to be 
    // modified or referenced until a new Sprite is created and this recycled ID can be assigned to
    // it.
    s_available_ids.emplace(id);

    _remove_texture_dependency(s_all_sprites.at(id).texture);
}

void SpriteHandler::set_sprite_scale_factor(float new_scale_factor)
{
    // Clamp the scale factor to a minimum of 1.0.
    m_sprite_scale_factor = Frost::clamp_float_to_minimum(new_scale_factor, 1.0f);
}

sprite_id SpriteHandler::create_sprite(uint16_t splice_x, uint16_t splice_y, uint16_t splice_w, 
    uint16_t splice_h, uint16_t dest_x, uint16_t dest_y, std::string png_path)
{ 
    sprite_id new_sprite_id = _get_next_id();

    // Get a reference to the Sprite that is going to be "created". The Sprite object is garunteed
    // to already exist in the vector since this is either a recycled id and the Sprite already 
    // exists, or it was just created on the last line since this is a new ID. Simply update the
    // values of the Sprite and allow it to be referenced that already exists to "create" a new 
    // one. 

    Sprite& target_sprite = s_all_sprites.at(new_sprite_id);

    target_sprite.splice_dimensions.x = splice_x;
    target_sprite.splice_dimensions.y = splice_y;
    target_sprite.splice_dimensions.w = splice_w;
    target_sprite.splice_dimensions.h = splice_h;
    
    target_sprite.display_dimensions.x = dest_x;
    target_sprite.display_dimensions.y = dest_y;
    target_sprite.display_dimensions.w = splice_w;
    target_sprite.display_dimensions.h = splice_h;

    target_sprite.texture = m_texture_handler->create_texture(png_path);

    // Register that this Texture has another dependency.
    ++s_texture_dependencies[target_sprite.texture];

    return new_sprite_id;
}

const Sprite& SpriteHandler::get_sprite(sprite_id id)
{
    if(!_is_id_valid(id)) 
    {
        // Crash the program, since no valid Sprite object exists for this ID.

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("SpriteHandler.get_sprite() -> Sprite ID : \"" + 
            std::to_string(id) + "\" does not exist.", Frost::ERR);
        #endif

        exit(1);
    }

    return s_all_sprites.at(id);
}

uint64_t SpriteHandler::get_size()
{
    uint64_t size {};

    size += s_all_sprites.capacity() * sizeof(Sprite);
    size += m_sprites_to_render.capacity() * sizeof(Sprite);

    return size;
}

// Private

void SpriteHandler::_remove_texture_dependency(SDL_Texture* texture)
{
    uint64_t& num_dependencies = s_texture_dependencies.at(texture);

    --num_dependencies;

    // If there are more Sprites who need this Texture.
    if(num_dependencies != 0) return; 
    
    s_texture_dependencies.erase(texture);

    m_texture_handler->handle_texture_deletion(texture);
}

void SpriteHandler::_place_id_in_rendering_ids(sprite_id id, uint16_t layer)
{
    // Find the index of the first layer that is not greater than the target layer.
    const std::vector<Layer>::iterator layer_it =
        std::lower_bound(
        m_sprites_to_render.begin(), 
        m_sprites_to_render.end(), 
        layer,
        [](const Layer& compare, uint16_t target_layer) 
            { return compare.layer < target_layer; }
    );

    // There are not any sprites being rendered on the given layer.
    if(m_existing_layers.count(layer) == 0)
    {
        // Update the existing layers to include this new layer.
        m_existing_layers.emplace(layer);

        // Create a new Layer object and insert it at its position.
        m_sprites_to_render.insert(layer_it, Layer {});

        // Update the new Layer object and add the sprite id to its tracked sprites.
        layer_it->layer = layer;
        layer_it->sprites.push_back(id);

        return;
    }

    // Vector of sprites corresponding to the target layer.
    std::vector<sprite_id>& target_layer_sprites = (*layer_it).sprites;

    // Find the position in the sprites vector of the target layer where
    const std::vector<sprite_id>::const_iterator sprites_it = std::lower_bound(
        target_layer_sprites.begin(), target_layer_sprites.end(), id);

    // Insert the id at its sorted position.
    target_layer_sprites.insert(sprites_it, id);
}

void SpriteHandler::_remove_id_from_rendering_ids(sprite_id id)
{
    

    // Find the position of the ID.

    const std::vector<sprite_id>::const_iterator it = 
        std::lower_bound(m_sprites_to_render.begin(), m_sprites_to_render.end(), id);

    // No error checking for if the ID exists in the vector, since the addition and deletion of IDs
    // from this vector are handled internally, and can be safely assumed that it exists if this
    // method is being called.

    // Erase the ID from the vector.
    m_sprites_to_render.erase(it);
}

bool SpriteHandler::_is_id_valid(sprite_id id)
{
    // If the ID of this Sprite doesn't exist or it is a recycled ID of a deleted Sprite.
    if(id >= s_all_sprites.size() || 
        s_available_ids.find(id) != s_available_ids.end()) return false;

    return true;
}

sprite_id SpriteHandler::_get_next_id()
{
    sprite_id available_id;

    // There are recycled IDs available.
    if(s_available_ids.size() > 0) 
    {
        available_id = *s_available_ids.begin();
        s_available_ids.erase(available_id);

        return available_id;
    }

    // Get the next ID in line, which is simply the size of the vector since the length of the 
    // vector corresponds directly to the number of Sprites (IDs) created.

    available_id = s_all_sprites.size();

    // Add a new Sprite to the Sprites vector. The new size
    s_all_sprites.push_back(Sprite {available_id});

    return available_id;
}

