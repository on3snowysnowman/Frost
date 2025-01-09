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
#include "Fr_Math.hpp"
#include "TextFileHandler.hpp"

// Static Members

std::vector<Sprite> SpriteHandler::s_all_sprites;

std::unordered_set<sprite_id> SpriteHandler::s_available_ids;

std::unordered_map<SDL_Texture *, uint64_t> SpriteHandler::s_texture_dependencies;

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

void SpriteHandler::_render()
{
    // Iterate through each active z layer.
    for (const uint16_t layer : m_active_layers)
    {
        // Iterate through the sprites on this layer.
        for (const sprite_id id : m_layers_to_sprites.at(layer))
        {
            Sprite &sprite = s_all_sprites.at(id);

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
    if (!_is_id_valid(id))
    {
        // Crash the program, since no valid Sprite object exists for this ID.

        TextFileHandler::add_to_buffer("[ERR] SpriteHandler.set_sprite_position() -> Sprite ID : \"" +
                std::to_string(id) + "\" does not exist.\n");
        TextFileHandler::write("CrashLog.txt", Frost::APPEND);
        exit(1);
    }

    Sprite& sprite = s_all_sprites.at(id);

    // Update the Sprite's position.
    sprite.display_dimensions.x = x;
    sprite.display_dimensions.y = y;
}

void SpriteHandler::modify_sprite_position(sprite_id id, int16_t delta_x, int16_t delta_y)
{
    if (!_is_id_valid(id))
    {
        // Crash the program, since no valid Sprite object exists for this ID.

        TextFileHandler::add_to_buffer("[ERR] SpriteHandler.modify_sprite_position() -> Sprite ID : \"" +
                std::to_string(id) + "\" does not exist.\n");
        TextFileHandler::write("CrashLog.txt", Frost::APPEND);
        exit(1);
    }

    Sprite& sprite = s_all_sprites.at(id);

    // Update the Sprite's position.
    sprite.display_dimensions.x += delta_x;
    sprite.display_dimensions.y += delta_y;
}

void SpriteHandler::flag_render(sprite_id id, uint16_t layer)
{
    // Don't allow duplicate rendering of a single Sprite.
    if (_is_sprite_rendering(id))
        return;

    // Find the position in the sorted vector where this layer would be at if it exists or where it
    // should be inserted if it does not.
    const std::vector<uint16_t>::const_iterator it =
        std::lower_bound(m_active_layers.begin(), m_active_layers.end(), layer);

    // This layer does not exist yet.
    if (it == m_active_layers.end() || *it != layer)
    {
        // Update the active layers to reflect this new layer.
        m_active_layers.insert(it, layer);

        // Create a new vector of sprites for this layer inside the map.
        m_layers_to_sprites.emplace(layer, std::vector<sprite_id>{});
    }

    _insert_id_in_layer_vector(id, layer);
    m_sprites_to_layers.emplace(id, layer);
}

void SpriteHandler::deflag_render(sprite_id id)
{
    const uint16_t target_layer = m_sprites_to_layers.at(id);

    // Erase the id from the map, since will no longer be rendering on a layer.
    m_sprites_to_layers.erase(id);

    // Get the vector of sprites at the layer this Sprite is rendered on.
    std::vector<sprite_id> &layer_vector =
        m_layers_to_sprites.at(target_layer);

    // This is the last sprite in the vector at this layer.
    if (layer_vector.size() == 1)
    {
        // Erase the layer from the map, since it no longer has any sprites to render.
        m_layers_to_sprites.erase(target_layer);
        return;
    }

    // Find the Sprite in the vector of Sprites at this layer.
    const std::vector<sprite_id>::const_iterator it =
        std::lower_bound(layer_vector.begin(), layer_vector.end(), id);

    // Erase the Sprite from the rendering vector at this layer.
    layer_vector.erase(it);
}

void SpriteHandler::delete_sprite(sprite_id id)
{
    if (!_is_id_valid(id))
    {
        // Crash the program, since no valid Sprite object exists for this ID.

        TextFileHandler::add_to_buffer("[ERR] SpriteHandler.delete_sprite() -> Sprite ID : \"" +
            std::to_string(id) + "\" does not exist.\n");
        TextFileHandler::write("CrashLog.txt", Frost::APPEND);
        exit(1);
    }

    if (_is_sprite_rendering(id))
        deflag_render(id);

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

    // Get a reference to the Sprite that is going to be "created". The Sprite object is guaranteed
    // to already exist in the vector since this is either a recycled id and the Sprite already
    // exists, or it was just created on the last line since this is a new ID. Simply update the
    // values of the Sprite and allow it to be referenced that already exists to "create" a new
    // one.

    Sprite &target_sprite = s_all_sprites.at(new_sprite_id);

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

const Sprite &SpriteHandler::get_sprite(sprite_id id)
{
    return s_all_sprites.at(id);
}


// Private

void SpriteHandler::_insert_id_in_layer_vector(sprite_id id, uint16_t layer)
{
    std::vector<sprite_id> &layer_vector = m_layers_to_sprites.at(layer);

    // Find the position in the sorted layer vector where this id should be inserted.
    const std::vector<sprite_id>::const_iterator it =
        std::lower_bound(layer_vector.begin(), layer_vector.end(), id);

    layer_vector.insert(it, id);
}

void SpriteHandler::_remove_texture_dependency(SDL_Texture *texture)
{
    uint64_t &num_dependencies = s_texture_dependencies.at(texture);

    --num_dependencies;

    // If there are more Sprites who need this Texture.
    if (num_dependencies != 0)
        return;

    s_texture_dependencies.erase(texture);

    m_texture_handler->handle_texture_deletion(texture);
}

bool SpriteHandler::_is_id_valid(sprite_id id)
{
    // If the ID of this Sprite doesn't exist or it is a recycled ID of a deleted Sprite.
    if (id >= s_all_sprites.size() ||
        s_available_ids.find(id) != s_available_ids.end())
        return false;

    return true;
}

bool SpriteHandler::_is_sprite_rendering(sprite_id id)
{
    // If the sprite_id has a layer associated with it, that means it is rendering on a layer.
    return m_sprites_to_layers.count(id);
}

sprite_id SpriteHandler::_get_next_id()
{
    sprite_id available_id;

    // There are recycled IDs available.
    if (s_available_ids.size() > 0)
    {
        available_id = *s_available_ids.begin();
        s_available_ids.erase(available_id);

        return available_id;
    }

    // Get the next ID in line, which is simply the size of the vector since the length of the
    // vector corresponds directly to the number of Sprites (IDs) created.

    available_id = s_all_sprites.size();

    // Add a new Sprite to the Sprites vector. The new size
    s_all_sprites.push_back(Sprite{});

    return available_id;
}
