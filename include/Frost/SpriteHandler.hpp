#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <SDL2/SDL_image.h>

#include "TextureHandler.hpp"
#include "Sprite.hpp"


class SpriteHandler
{

public:

    SpriteHandler();

    SpriteHandler(TextureHandler* texture_handler);

    /** Renders Sprites flagged to render to the screen. */
    void render();

    /** Sets the position of a Sprite on the screen to a new position.
     * 
     * @param id ID of the Sprite to change.
     * @param x X position in pixels.
     * @param y Y position in pixels.
     */
    void set_sprite_position(sprite_id id, uint16_t x, uint16_t y);

    /** Flags a Sprite to render each frame. Only call this method once to begin rendering the
     * Sprite. To cease rendering, call the deflag_render method. 
     * 
     * @param id ID of the Sprite to render.
     */
    void flag_render(sprite_id id);

    /** Flags a Sprite to stop rendering. 
     * 
     * @param id ID of the Sprite to stop rendering.
     */
    void deflag_render(sprite_id id);

    /** Deletes the Sprite assigned to the passed ID, and frees the ID so that it is able to be
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

    uint64_t get_size();


private:

    // Members

    // Factor that each Sprite will be upscaled by when rendered. 
    float m_sprite_scale_factor = 1.0f;

    // Vector of Sprites (Resembled as their ID's) to render each frame. Sorted low to high.
    std::vector<sprite_id> m_sprites_to_render;

    /** All Sprites that have been created. The index of the vector corresponds with the Sprite's 
     * ID. */
    static std::vector<Sprite> s_all_sprites;

    // IDs of Sprites that have been deconscructed and are available to recycle and reuse.
    static std::unordered_set<sprite_id> s_available_ids;

    // Tracks the number of Sprites that require each Texture.
    static std::unordered_map<SDL_Texture*, uint64_t> s_texture_dependencies;


    TextureHandler* m_texture_handler;


    // Methods

    /** Deducts 1 from the number of tracked Sprite dependencies from the passed Texture. If the 
     * number of dependencies reaches 0, the Texture is removed from the map and is deleted from 
     * heap memory using the TextureHandler.
     */
    void _remove_texture_dependency(SDL_Texture* texture);
    
    /** Adds the passed ID in the Sprite rendering vector in the correct position sorted by value.
     * 
     * @param id ID to place.
     */
    void _place_id_in_rendering_ids(sprite_id id);

    /** @brief Removes the id from the m_sprites_to_render vector.
     * 
     * @param id ID to remove.
     */
    void _remove_id_from_rendering_ids(sprite_id id);

    /** Returns true if the Sprite assigned to the passed ID is existant and available. 
     * 
     * @param id ID to check.
    */
    static bool _is_id_valid(sprite_id id);

    /** Gets the next available ID for a new Sprite, whether that be a recycled ID from a Sprite
     * that has been deleted, or a new ID that doesn't exist.
     */
    static sprite_id _get_next_id();
};

