#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "SpriteHandler.hpp"
#include "CrashOutputHandler.hpp"
#include "Fr_IntLimits.hpp"


// Constructors / Deconstructor

SpriteHandler::SpriteHandler() {}

SpriteHandler::SpriteHandler(TextureHandler* texture_handler, 
    DrawHandler* draw_handler) 
{
    m_texture_handler = texture_handler;
    m_draw_handler = draw_handler;
}

void SpriteHandler::_render() 
{
    for(uint16_t layer : m_active_layers)
    {
        for(const SpriteInstanceData& instance_data : 
            m_layers_to_sprites.at(layer))
        {
            m_draw_handler->draw(instance_data.target_sprite->texture,
                instance_data.target_sprite->splice_dimensions,
                instance_data.display_dimensions);
        }
    }
}


// Public

void SpriteHandler::set_instance_position(rendering_id ID, int x,
    int y) 
{
    if(!_is_ID_valid_and_rendering(ID))
    {
        OUTPUT_CRASH_DETAILS(" where 'ID' = '" + std::to_string(ID) + 
            "' : ID does not exist or is not rendering.");
        exit(1);
    }

    SpriteInstanceData& instance_data = _get_instance(ID);

    instance_data.display_dimensions.x = x;
    instance_data.display_dimensions.y = y;
}

void SpriteHandler::modify_instance_position(rendering_id ID, 
    int delta_x, int delta_y) 
{
    if(!_is_ID_valid_and_rendering(ID))
    {
        OUTPUT_CRASH_DETAILS(" where 'ID' = '" + std::to_string(ID) + 
            "' : ID does not exist or is not rendering.");
        exit(1);
    }

    SpriteInstanceData& instance_data = _get_instance(ID);

    instance_data.display_dimensions.x += delta_x;
    instance_data.display_dimensions.y += delta_y;
}

void SpriteHandler::unrender_sprite(rendering_id ID) 
{   
    if(!_is_ID_valid_and_rendering(ID))
    {
        OUTPUT_CRASH_DETAILS(" where 'ID' = '" + std::to_string(ID) + 
            "' : ID does not exist or is not rendering.");
        exit(1);
    }

    // Handle the removing of the instance associated with this ID.
    _remove_instance(ID);
}

void SpriteHandler::delete_sprite(Sprite& sprite) 
{
    // No checking is done here for if any render instance of this Sprite 
    // exists. The user is warned in the docstring of the function declaration
    // of this behavior. It is the user's responsiblility to ensure that any
    // instance of this Sprite is unrendered before its deletion.

    m_texture_handler->handle_texture_deletion(sprite.texture);
    sprite.texture = nullptr;
}

rendering_id SpriteHandler::render_sprite(const Sprite& sprite, 
    uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t layer) 
{
    // Create the new instance, immediately initializing the const members.
    SpriteInstanceData new_instance
    {
        &sprite,
        _get_unique_id(),
        SDL_Rect {}
    };

    // new_instance.target_sprite = &sprite;

    // new_instance.ID = _get_unique_id();

    new_instance.display_dimensions.x = x;
    new_instance.display_dimensions.y = y;

    if(w == 0) w = sprite.splice_dimensions.w;
    if(h == 0) h = sprite.splice_dimensions.h;
 
    new_instance.display_dimensions.w = w;
    new_instance.display_dimensions.h = h;
    
    _place_sprite_instance(layer, new_instance);

    return new_instance.ID;
}

Sprite SpriteHandler::create_sprite(const char* texture_path, 
    uint16_t splice_x, uint16_t splice_y, uint16_t splice_w, uint16_t splice_h) 
{
    Sprite new_sprite;

    new_sprite.splice_dimensions = SDL_Rect {splice_x, splice_y, splice_w, 
        splice_h};

    new_sprite.texture = m_texture_handler->create_texture(
        std::string(texture_path));

    return new_sprite;
}

SpriteInstanceData& SpriteHandler::get_instance_data(rendering_id ID)
{
    if(!_is_ID_valid_and_rendering(ID))
    {
        OUTPUT_CRASH_DETAILS(" where 'ID' = '" + std::to_string(ID) + 
            "' : ID does not exist or is not rendering.");
        exit(1);
    }

    return _get_instance(ID);
}

// Private

void SpriteHandler::_place_sprite_instance(uint16_t layer, 
    const SpriteInstanceData& instance_data)
{
    // Update the tracking of rendering ideas to reflect that this ID is now
    // active on this layer.
    m_ren_ids_to_layer[instance_data.ID] = layer;

    std::cout << "Placed: " << instance_data.ID << " on: " << layer << '\n';

    // Find the layer vector, if it exists.
    std::unordered_map<uint16_t, std::vector<SpriteInstanceData>>::iterator it 
        = m_layers_to_sprites.find(layer);

    // If this layer has not been used yet.
    if(it == m_layers_to_sprites.end())
    {
        m_layers_to_sprites.emplace(layer, std::vector<SpriteInstanceData> 
            {instance_data});

        // Since this is a new layer and we're placing a new instance on it, it
        // is known that this layer was not active before, and needs to be 
        // added to the tracked active layers vector since this new instance 
        // is now rendering on it.
        m_active_layers.push_back(layer);
        return;
    }

    // If this layer is empty, and is now active as we're placing an instance 
    // on it.
    if(it->second.size() == 0)
    {
        m_active_layers.push_back(layer);
    }

    // Place the instance in the vector at this layer.
    it->second.push_back(instance_data);
}

void SpriteHandler::_remove_instance(rendering_id ID)
{
    // Add this ID to the available IDs, since it is now decomissioned.
    m_available_rendering_ids.push(ID);

    // Find the layer this ID is rendering on.
    uint16_t targ_layer = m_ren_ids_to_layer.at(ID);

    // Get the vector of instances at the layer this ID is rendering on.
    std::vector<SpriteInstanceData>& targ_vector = 
        m_layers_to_sprites.at(targ_layer);

    // Remove the instance from the vector
    targ_vector.erase(_find_instance_in_vector(ID, targ_vector));

    // Set the layer that this ID is rendering on to the limit of a uint16, 
    // which will flag the SpriteHandler that this ID is no longer rendering on
    // any layer if it needs to check.
    m_ren_ids_to_layer.at(ID) = Frost::UINT16_LIMIT;

    // If this vector is not empty, the layer is still active, so do nothing.
    if(targ_vector.size() != 0) return;

    // The vector is empty, meaning no more instances are rendering on it so it
    // is now inactive. Remove it from the active layers vector.

    m_active_layers.erase(
        std::lower_bound(
            m_active_layers.begin(),
            m_active_layers.end(),
            targ_layer)
    );
}

bool SpriteHandler::_is_ID_valid_and_rendering(rendering_id ID)
{
    return m_ren_ids_to_layer.find(ID) != m_ren_ids_to_layer.end() && 
        m_ren_ids_to_layer.at(ID) < Frost::UINT16_LIMIT;
}

rendering_id SpriteHandler::_get_unique_id()
{
    // There are decomissioned IDs available.
    if(m_available_rendering_ids.size() != 0)
    {
        rendering_id ID = m_available_rendering_ids.front();
        m_available_rendering_ids.pop();

        return ID;
    }

    // No decomissioned IDs are available, generate a new one.
    return m_next_id++;

}

std::vector<SpriteInstanceData>::iterator 
    SpriteHandler::_find_instance_in_vector(rendering_id ID, 
    std::vector<SpriteInstanceData>& vec)
{
    // Return an iterator to the instance with the matching ID.
    return std::lower_bound(
        vec.begin(),
        vec.end(),
        ID,
        [](SpriteInstanceData& instance, rendering_id ID) 
            { return instance.ID < ID;}
    );
}

SpriteInstanceData& SpriteHandler::_get_instance(
    rendering_id ID)
{
    uint16_t target_layer;

    // Attempt to retrieve the layer this ID is rendering on.
    try
    {
        target_layer = m_ren_ids_to_layer.at(ID);
    }

    // Rendering ID is invalid.
    catch(const std::out_of_range& e)
    {
        OUTPUT_CRASH_DETAILS(" where 'ID' = '" + std::to_string(ID) + 
            "' : ID does not exist.");
        exit(1);
    }

    // Get the vector of instances at this layer.
    std::vector<SpriteInstanceData>& target_vector = 
        m_layers_to_sprites.at(target_layer);

    // Fetch an iterator to the instance and dereference it to get said instance.
    return *_find_instance_in_vector(ID, target_vector);
}