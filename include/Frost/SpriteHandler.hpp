#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>

#include <SDL_image.h>
#include <queue>

#include "TextureHandler.hpp"
#include "DrawHandler.hpp"
#include "Sprite.hpp"

using rendering_id = uint64_t;

/**
 * @brief Contains data for a Sprite instance that is being rendered to the 
 * screen.
 * 
 * The SpriteHandler uses these structs as ways to track instances of a Sprite
 * that is rendered to the screen.
 */
struct SpriteInstanceData
{
    // Sprite that this instance will use to render.
    const Sprite* target_sprite;

    // Unique ID of this rendering instance.
    rendering_id ID;

    // Dimensions to render the target sprite to the screen.
    SDL_Rect display_dimensions;
};


/**
 * @brief Manages Sprite creation and rendering. 
 * 
 * Sprites are created through the `create_sprite` method and deleted through 
 * the `delete_sprite` method. After obtaining the
 * Sprite, the user may modify the contents of it during runtime while it is 
 * being rendered.
 * 
 * To render a sprite, call the `render_sprite` method. This method returns a 
 * unique id, a `rendering_id`. `rendering_id`s are unique ids of a specific 
 * 'instance' of the Sprite that is being rendered. A single sprite can be 
 * rendered at an arbitrary amount of positions, this creates an 'instance' of
 * that Sprite on the screen. For each instance, a pointer is maintained to the
 * Sprite that the instance is rendering from. Users may obtain the specific 
 * instance data using the unique `rendering_id` of the instance through the
 * `get_instance_data` method. This method returns a reference to the data,
 * meaning the user may freely alter the display dimensions of the instance 
 * outside of the SpriteHandler to modify the position of the instance on the
 * screen. Additionally, an instance's position may be altered through the 
 * `set_instance_position` and `modify_instanced_position` methods.
 * 
 */
class SpriteHandler

{

public:


    SpriteHandler();

    SpriteHandler(TextureHandler* texture_handler, DrawHandler* draw_handler);

    /**
     * @brief Renders all Sprite instances to the screen.
     * 
     * This method is invoked internally by the Frost Engine every frame.
     * Users should avoid calling this method.
     */
    void _render();

    /**
     * @brief Sets the position of a Sprite instance associated with the 
     * specified rendering ID.
     * 
     * This method adjusts the position of the Sprite within its current 
     * rendering layer.
     * 
     * @param id The unique rendering ID of the Sprite instance.
     * @param x The new x-coordinate for the instance's position.
     * @param y The new y-coordinate for the intsance's position.
     */
    void set_instance_position(rendering_id id, int x, int y);

    /**
     * @brief Adjusts the position of a Sprite instance associated with the
     * specified rendering ID by applying the given deltas.
     * 
     * The delta values are added to the current position of the Sprite. 
     * If a delta is 0, the corresponding dimension remains unchanged.
     * 
     * @param id The unique rendering ID of the Sprite instance.
     * @param delta_x The change in the x-coordinate.
     * @param delta_y The change in the y-coordinate.
     */
    void modify_instance_position(rendering_id id, int delta_x,
        int delta_y);

    /**
     * @brief Deletes a Sprite instance associated with the given rendering ID 
     * and releases the rendering ID for reuse.
     * 
     * @param id The unique rendering ID of the Sprite instance to delete.
     */
    void unrender_sprite(rendering_id id);

    /**
     * @brief Deletes the specified Sprite and manages its associated texture 
     * through the TextureHandler.
     * 
     * Use this method to delete a Sprite if it was created by this 
     * SpriteHandler. The TextureHandler internally tracks SDL_Texture* 
     * dependencies to determine whether the texture can be safely removed 
     * from memory or if it is still in use by other objects.
     * 
     * This method leaves `sprite` in an invalid state with no texture. Either
     * discard the object or reassign the Sprite to a new Sprite using the 
     * `create_sprite` method.
     * 
     * @warning Ensure all instances of the Sprite are removed using 
     * `unrender_sprite` before calling this method. The SpriteHandler does 
     * not track which Sprite instances are tied to a specific Sprite object 
     * and will not automatically handle their removal.
     * 
     * @param sprite The Sprite object to delete.
     */
    void delete_sprite(Sprite& sprite);

    /**
     * @brief Creates and renders a Sprite instance at the specified position 
     * and layer, returning its unique rendering ID.
     * 
     * @param sprite The Sprite instance to render.
     * @param layer The rendering layer where the Sprite will be placed. 
     *              Defaults to 0 if not specified.
     * @param x The x-coordinate of the Sprite's position.
     * @param y The y-coordinate of the Sprite's position.
     * @param w Width to render the Sprite at. Leave this 0 to render at the 
     *          splice width.
     * @param h Height to render the SPrite at. Leave this 0 to render at the 
     *          splice height.
     * @return rendering_id The unique rendering ID of the created Sprite 
     * instance.
     */
    rendering_id render_sprite(const Sprite& sprite, uint16_t x,uint16_t y, 
        uint16_t w = 0, uint16_t h = 0, uint16_t layer = 0);

    /**
     * @brief Creates a Sprite object from a texture file and specified splicing 
     * and display dimensions.
     * 
     * @param texture_path The file path to the texture image.
     * @param splice_x The x-coordinate of the top-left corner of the spliced 
     *                 region.
     * @param splice_y The y-coordinate of the top-left corner of the spliced 
     *                 region.
     * @param splice_w The width of the spliced region.
     * @param splice_h The height of the spliced region.
     * 
     * @return Sprite The created Sprite object.
     */
    Sprite create_sprite(const char* texture_path, uint16_t splice_x, 
        uint16_t splice_y, uint16_t splice_w, uint16_t splice_h);

    SpriteInstanceData& get_instance_data(rendering_id ID);

private:

    // Members

    TextureHandler* m_texture_handler;
    DrawHandler* m_draw_handler;

    // Decomissioned rendering IDs that are available for use again.
    std::queue<rendering_id> m_available_rendering_ids;

    // /**
    //  * A 2D vector of sprites to render. Each index of the outer vector 
    //  * represents a 'z layer' for rendering sprites, enabling layered rendering.
    //  */
    // std::vector<std::vector<SpriteInstanceData>> m_sprites_to_render;

    /** Layers that currently contain sprite instances that are rendering. */
    std::vector<uint16_t> m_active_layers;

    /** 
     * @brief Maps layers to their list of Sprite instances currently rendering
     * on them. 
     * 
     * Once a layer is added to this map, its associated vector persists for 
     * the lifetime of the program, even if emptied. This design avoids the 
     * overhead of repeatedly constructing and rehashing vectors for layers 
     * that are frequently emptied and refilled.
     */
    std::unordered_map<uint16_t, std::vector<SpriteInstanceData>>
        m_layers_to_sprites;

    /**
     * Maps unique rendering IDs to the corresponding layer index
     * where the Sprite instance is rendered.
     */
    std::unordered_map<rendering_id, uint16_t> m_ren_ids_to_layer;

    /** The next rendering ID to be generated if no decommissioned IDs are 
     * available.
     */
    rendering_id m_next_id = 0;

    // Methods

    /**
     * @brief Places a Sprite instance in its sorted position within the 
     * respective layer vector.
     * 
     * @param layer Layer to place the instance at.
     * @param instance_data Sprite instance to place.
     */
    void _place_sprite_instance(uint16_t layer, 
        const SpriteInstanceData& instance_data);

    /**
     * @brief Removes the Sprite instance associated with the given rendering ID 
     * from its current layer.
     * 
     * If the layer becomes empty after removal, it is inactive and 
     * removed from the active layers vector.
     * 
     * @param ID The unique rendering ID of the Sprite instance to remove.
     */
    void _remove_instance(rendering_id ID);

    /**
     * @brief Places a layer into the `m_active_layers` vector at a position 
     * that maintains descending order.
     * 
     * @param layer Layer to place.
     */
    void _place_active_layer(uint16_t layer);

    /**
     * @brief Returns true if `ID` is valid and is currently rendering.
     * 
     * @param ID ID to check.
     * @return true 
     * @return false 
     */
    bool _is_ID_valid_and_rendering(rendering_id ID);

    /**
     * @brief Retrieves an available unique rendering ID, either from the pool 
     * of decommissioned IDs or by generating a new one.
     * 
     * @return rendering_id The unique rendering ID.
     */
    rendering_id _get_unique_id();

    std::vector<SpriteInstanceData>::iterator _find_instance_in_vector(
        rendering_id ID, std::vector<SpriteInstanceData>& vec);

    /**
     * @brief Retrieves the instance data associated with the specified 
     * rendering ID.
     * 
     * @param ID The unique rendering ID of the Sprite instance.
     * @return const SpriteInstanceData& A reference to the Sprite instance 
     *         data.
     */
    SpriteInstanceData& _get_instance(rendering_id ID);
};
