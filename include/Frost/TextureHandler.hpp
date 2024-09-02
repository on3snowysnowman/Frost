#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>

#include <SDL2/SDL_image.h>

#include "Color.hpp"


/** Used for creating SDL_Textures, along with drawing these textures to the screen using SDL. 
 * The paths to textures created are tracked, such that if two requests are made to create a 
 * texture from the same path, the texture will be created one time and will be fetched the second
 * time. This is an underlying class, used by other classes to provide meaningful output to the 
 * screen like the TextRenderingHandler or SpriteHandler. 
*/
class TextureHandler
{

public:

    TextureHandler();

    TextureHandler(SDL_Renderer* renderer, std::string color_data_path);

    ~TextureHandler();

    /** Draws a portion of the passed texture to the screen. The portion of the texture is 
     * determined by the passed source SDL_Rect, while the dimensions on the screen is determined
     * by the passed dest SDL_Rect.
     * 
     * @param texture Texture to draw.
     * @param source Dimensions to portion from the texture.
     * @param dest Dimensions to place on the screen.
     */
    void draw(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& dest) const;

    /** Draws a portion of the passed texture to the screen. The portion of the texture is 
     * determined by the passed source SDL_Rect, while the dimensions on the screen is determined
     * by the passed dest SDL_Rect. The passed color is applied to the texture.
     * 
     * @param texture Texture to draw.
     * @param source Dimensions to portion from the texture.
     * @param dest Dimensions to place on the screen.
     * @param color Color to draw the texture with.
     */
    void draw(SDL_Texture* texture, const SDL_Rect& source, 
        const SDL_Rect& dest, const std::string color) const;

    /** Deletes the SDL_Texture and removes it from internal components.
     * 
     * @param texture SDL_Texture to delete.
    */
    void handle_texture_deletion(SDL_Texture* texture);

    /** Returns a const reference to the internal map of colors. */
    const std::unordered_map<std::string, Color>& get_colors() const;

    /** Creates and returns an SDL_Texture from the png at the passed path. If a texture has 
     * already been created from this path, it is fetched. 
     * 
     * @param png_path Path to the png.
     */
    SDL_Texture* create_texture(std::string png_path) const;

private:

    // Members

    // Path to the color data file.
    std::string m_color_data_path;

    // Color names to their Color objects.
    std::unordered_map<std::string, Color> m_colors;

    // Texture objects to the path they have been created from.
    static std::unordered_map<SDL_Texture*, std::string> s_textures_to_paths;

    // Texture paths that have already had a texture created from them.
    static std::unordered_map<std::string, SDL_Texture*> s_paths_to_textures;

    SDL_Renderer* m_renderer;

    
    // Methods

    /** Creates and registers Color objects from the color data file. */
    void _get_colors_from_disk();
};
