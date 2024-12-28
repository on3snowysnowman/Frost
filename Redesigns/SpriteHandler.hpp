/**
 * @file SpriteHandler.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class declaration.
 * @version 0.2
 * @date 12-28-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <SDL_image.h>

#include "Sprite.hpp"
#include "TextureHandler.hpp"


/** 
 * @brief Handles tracking created Sprites and rendering them to the screen.
 */
class SpriteHandler
{

public:

    SpriteHandler();

    SpriteHandler(TextureHandler* texture_handler);
    
    /** Renders all Sprites that have been flagged to render. */
    void render();
    
    /** Sets the position of a Sprite on the screen to a new position.
     * 
     * @param id ID of the Sprite to change.
     * @param x X position in pixels.
     * @param y Y position in pixels.
     */
    void set_sprite_position(sprite_id id, uint16_t x, uint16_t y);
    
    /** 
     * @brief Flags a Sprite to render each frame on a specified Z layer.
     * 
     * Only call this method once to begin rendering the Sprite. To cease rendering, call the 
     * deflag_render method. 
     * 
     * @param id ID of the Sprite to render.
     * @param layer Z layer to render it on.
     */
    void flag_render(sprite_id id, uint16_t layer);

    /** Flags a Sprite to stop rendering. 
     * 
     * @param id ID of the Sprite to stop rendering.
     */
    void deflag_render(sprite_id id);

    /** Decommissions the Sprite assigned to the passed ID, and frees the ID so that it is able to be
     * reused. 
     * 
     * @param id ID of the Sprite to delete.
     */
    void delete_sprite(sprite_id id);

    /** Sets the scale factor for Sprite upscaling to a new value. This value has a minimum value 
     * of 1.0.
     */
    void set_sprite_scale_factor(float new_scale_factor);

    /** Creates a Sprite and assigns an ID to it. Returns the ID of the Sprite created. 
     * 
     * @param splice_x X position in the texture to start the splice at.
     * @param splice_y Y position in the texture to start the splice at.
     * @param splice_w Width of the splice of the texture.
     * @param splice_h Height of the splice of the texture.
     * @param dest_x X position to render the Sprite on screen.
     * @param dest_y Y position to render the Sprite on screen.
     * @param png_path Path to the png to create the Sprite's texture.
    */
    sprite_id create_sprite(uint16_t splice_x, uint16_t splice_y, uint16_t splice_w, 
        uint16_t splice_h, uint16_t dest_x, uint16_t dest_y, std::string png_path);

    /** Returns a const reference to the Sprite assigned to the passed ID.
     * 
     * @param id Id of the Sprite to fetch.
     */
    static const Sprite& get_sprite(sprite_id id);

    /** Called on program termination only, handles deletion of SDL_Textures and Sprites. */
    static void _cleanup();

private:

    // Members

    // Factor that each Sprite will be upscaled by when rendered. 
    float m_sprite_scale_factor = 1.0f;

    // Active Z layers that currently being rendered on.
    std::vector<uint16_t> m_active_layers;

    // Z layers to their Sprites that are active on them. 
    std::unordered_map<uint16_t, std::vector<sprite_id>> m_layers_to_sprites;

    // Active Sprite ID to its respective layer it is being rendered on.
    std::unordered_map<sprite_id, uint16_t> m_sprites_to_layers;

    /** All Sprites that have been created. The index of the vector corresponds with the Sprite's 
     * ID. */
    static std::vector<Sprite> s_all_sprites;

    // IDs of Sprites that have been deconstructed and are available to recycle and reuse.
    static std::unordered_set<sprite_id> s_available_ids;

    // Tracks the number of Sprites that require each Texture.
    static std::unordered_map<SDL_Texture*, uint64_t> s_texture_dependencies;

    TextureHandler* m_texture_handler;


    // Methods
    
    /** Inserts the sprite_id in its correct sorted position inside the vector of sprite_ids of the
     * respective layer. */
    void _insert_id_in_layer_vector(sprite_id id, uint16_t layer);

    /** Returns true if the Sprite assigned to the passed ID is existant and available. */
    static bool _is_id_valid(sprite_id id);

    /** Returns true if the Sprite assigned to the passed ID is currently rendering.  */
    static bool _is_sprite_rendering(sprite_id id);

    /** Gets the next available ID for a new Sprite, whether that be a recycled ID from a Sprite
     * that has been deleted, or a new ID that doesn't exist. */
    static sprite_id _get_next_id();
};
