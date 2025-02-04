#pragma once

#include <cstring>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>

#include <SDL_image.h>
#include <SDL_ttf.h>


// Characters that are supported for rendering by the Engine. 
static constexpr const char* RENDERABLE_CHARACTERS = "!\"#$%&'()*+,-./0123456789:;<=>?@"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

// Number of renderable characters. 
static constexpr std::size_t NUM_RENDERABLE_CHARS = std::strlen(RENDERABLE_CHARACTERS);

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

    TextureHandler(SDL_Renderer* renderer);

    ~TextureHandler();
    
    /** Deletes the SDL_Texture and removes it from internal components.
     * 
     * @param texture SDL_Texture to delete.
    */
    void handle_texture_deletion(SDL_Texture* texture);

    bool create_png_from_static_texture(SDL_Texture* staticTexture, const std::string& filePath);

    /** Creates and returns an SDL_Texture from the png at the passed path. If a texture has 
     * already been created from this path, it is fetched. 
     * 
     * @param png_path Path to the png.
     */
    SDL_Texture* create_texture(std::string png_path) const;

    /** 
     * @brief Given a font path, returns an atlas texture containing all 
     * rendered glyphs in a row. 
     * 
     * The `font_width` and `font_height` parameters are pointers, and will be 
     * set to the created font's dimensions. 
     */
    SDL_Texture* create_font_atlas_texture(std::string font_path, 
        int font_point_size, uint16_t& font_width, uint16_t& font_height);

private:

    // Members

    // Texture objects to the path they have been created from.
    static std::unordered_map<SDL_Texture*, std::string> s_textures_to_paths;

    // Texture paths that have already had a texture created from them.
    static std::unordered_map<std::string, SDL_Texture*> s_paths_to_textures;

    SDL_Renderer* m_renderer;

    
    // Methods

    /** 
     * @brief Given an open font, iterates through each renderable character to
     * calculate the font's final width and height.
     */
    void _calculate_font_dimensions(TTF_Font* font, uint16_t& font_width, 
        uint16_t& font_height);
};
