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

/** 
 * 
 * @brief Used for creating SDL_Textures, including making font atlas textures.
 * 
 * When the `create_texture()` method is called, the TextureHandler checks if 
 * a request has already been made to create a texture from this path. If there 
 * has been a request made already, said Texture will be fetched and returned,
 * instead of creating a new Texture from the path. The TextureHandler tracks 
 * the number of 'dependencies' the rely on a texture. The method for doing so
 * is quite primitive, it simply keeps a numerical track of each time the 
 * `create_texture()` method is called. For each `handle_texture_deletion()`
 * method call, the number of dependencies is deducted by 1. This means that 
 * the caller must be careful in how many times the `create_texture()` method
 * is called. If it is called twice, but deletion is only called once,
 * the SDL_Texture will continute to sit in GPU memory until the second 
 * deletion call is made to match the two creation calls (Two established 
 * dependencies). 
 * 
 * The `handle_texture_deletion()` is NOT guaranteed to delete the Texture.
 * The Texture will only be deleted from the GPU if that call was the last 
 * logged dependency. If the Texture were deleted and other objects were 
 * sharing that Texture (since they also requested a Texture to be created
 * from the same path, as specified previously), this would invoke undefined
 * behavior from dangling pointers. 
 * 
 * @attention SDL_Textures are created in GPU VRAM, and must be deleted 
 * properly! Simply using `delete` on the pointer is not sufficient and will 
 * lead to GPU memory leaks, along with dangling pointers if other objects are
 * sharing the SDL_Texture* that was deleted. If an SDL_Texture has reached the 
 * end of its service, call the `handle_texture_deletion()`. 
*/
class TextureHandler
{

public:

    TextureHandler();

    TextureHandler(SDL_Renderer* renderer);

    ~TextureHandler();
    
    /** 
     * @brief Deducts 1 from the tracked dependencies of the texture, and if 
     * that is now 0, deletes the SDL_Texture* from GPU VRAM along with the
     * pointer on heap. 
     * 
     * If an SDL_Texture* is passed through this method that was not created
     * using the TextureHandler, the Texture will be properly deleted 
     * regardless.
     * 
     * @param texture SDL_Texture to handle.
    */
    void handle_texture_deletion(SDL_Texture* texture);

    bool create_png_from_static_texture(SDL_Texture* staticTexture, const std::string& filePath);

    /** 
     * @brief Returns an SDL_Texture from the png at the passed path. 
     * 
     * If a Texture has already been created from this path, it is fetched
     * and another dependency is logged for the Texture.
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

    // Texture to the path they have been created from.
    static std::unordered_map<SDL_Texture*, std::string> s_textures_to_paths;

    // Textures to the number of dependencies that rely on them.
    static std::unordered_map<SDL_Texture*, uint64_t> 
        s_textures_to_dependencies;

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
