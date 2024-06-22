#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <SDL2/SDL_image.h>

#include "Color.h"


/** Used for creating SDL_Textures, along with drawing these textures to the screen using SDL. 
 * This is an underlying class, used by other classes to provide meaningful output to the screen 
 * like the ConsoleOutputHandler or SpriteHandler. 
*/
class TextureHandler
{

public:

    TextureHandler();

    TextureHandler(SDL_Renderer* renderer, std::string color_data_path);

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

    /** Returns a const reference to the internal map of colors. */
    const std::unordered_map<std::string, Color>& get_colors() const;

    /** Creates and returns an SDL_Texture from the png at the passed path.
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

    SDL_Renderer* m_renderer;

    
    // Methods

    /** Creates and registers Color objects from the color data file. */
    void _get_colors_from_disk();
};
