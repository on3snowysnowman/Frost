/**
 * @file TextureHandler.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class implementation.
 * @version 0.1
 * @date ?
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "TextureHandler.hpp"
#include "JsonHandler.hpp"
#include "FileSystemHandler.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif

// Static Members

std::unordered_map<SDL_Texture*, std::string> TextureHandler::s_textures_to_paths;

std::unordered_map<std::string, SDL_Texture*> TextureHandler::s_paths_to_textures;




// Constructors / Deconstructor

TextureHandler::TextureHandler() 
{
    m_renderer = nullptr;
}

TextureHandler::TextureHandler(SDL_Renderer* renderer, std::string color_data_path)
{
    m_renderer = renderer;
    m_color_data_path = color_data_path;

    // Get colors. 
    _get_colors_from_disk();
}

TextureHandler::~TextureHandler()
{
    for(std::pair<SDL_Texture*, std::string> _pair : s_textures_to_paths)
    {
        SDL_DestroyTexture(_pair.first);
    }
}


// Public

void TextureHandler::draw(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& dest) const
{
    SDL_RenderCopy(m_renderer, texture, &source, &dest);
}

void TextureHandler::draw(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& dest, 
    const std::string color) const
{
    // If this color isn't registered.
    if(m_colors.find(color) == m_colors.end())
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("TextureHandler.draw() -> Color: \"" + color 
            + "\" is not a registered color", Frost::ERR);
        #endif

        exit(1);
    }

    // Color object respective to the passed color name.
    const Color& targ_color = m_colors.at(color);

    // Store the original color values of the texture, since the texture's color channels must be
    // modified during the rendering process, and needs to be restored after to their original value.

    SDL_Color original_texture_color;
    SDL_GetTextureColorMod(texture, &original_texture_color.r, &original_texture_color.g, 
        &original_texture_color.b);

    // Apply the color to the texture.
    SDL_SetTextureColorMod(texture, targ_color.r, targ_color.g, targ_color.b);

    // Copy the texture into the renderer.
    SDL_RenderCopy(m_renderer, texture, &source, &dest);

    // Restore the texture to its original color.
    SDL_SetTextureColorMod(texture, original_texture_color.r, original_texture_color.g,
        original_texture_color.b);
}

void TextureHandler::handle_texture_deletion(SDL_Texture* texture)
{
    // If this Texture was not found.
    if(s_textures_to_paths.find(texture) == s_textures_to_paths.end())
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("TextureHandler.handle_texture_deletion() -> Attempted to delete"
            " an SDL_Texture that does not exist.", Frost::WARN);
        #endif

        return;
    }

    SDL_DestroyTexture(texture);

    s_paths_to_textures.erase(s_textures_to_paths.at(texture));
    s_textures_to_paths.erase(texture);
}

bool TextureHandler::create_png_from_static_texture(SDL_Texture* staticTexture, const std::string& filePath) 
{
    // Written by ChatGPT

    if (!m_renderer || !staticTexture) {
        // std::cerr << "Invalid renderer or static texture.\n";
        return false;
    }

    // Step 1: Query the static texture's dimensions
    int width, height;
    SDL_QueryTexture(staticTexture, nullptr, nullptr, &width, &height);

    // Step 2: Create a new targetable texture
    SDL_Texture* targetTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!targetTexture) {
        // std::cerr << "Failed to create targetable texture: " << SDL_GetError() << "\n";
        return false;
    }

    // Save previous Render draw color.
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(m_renderer, &r, &g, &b, &a);

    // Step 3: Copy the static texture to the targetable texture
    SDL_SetRenderTarget(m_renderer, targetTexture);
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, staticTexture, nullptr, nullptr);
    SDL_SetRenderTarget(m_renderer, nullptr);
    
    // Reistablish Render draw color.
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);

    // Step 4: Create a surface to store the pixel data
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        // std::cerr << "Failed to create surface: " << SDL_GetError() << "\n";
        SDL_DestroyTexture(targetTexture);
        return false;
    }

    // Step 5: Read pixels from the targetable texture
    SDL_SetRenderTarget(m_renderer, targetTexture);
    if (SDL_RenderReadPixels(m_renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, surface->pixels, surface->pitch) != 0) {
        // std::cerr << "Failed to read pixels: " << SDL_GetError() << "\n";
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(targetTexture);
        SDL_SetRenderTarget(m_renderer, nullptr);
        return false;
    }
    SDL_SetRenderTarget(m_renderer, nullptr);

    // Step 6: Save the surface as a PNG file
    if (IMG_SavePNG(surface, filePath.c_str()) != 0) {
        // std::cerr << "Failed to save PNG: " << IMG_GetError() << "\n";
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(targetTexture);
        return false;
    }

    // Cleanup
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(targetTexture);

    return true;
}


const std::unordered_map<std::string, Color>& TextureHandler::get_colors() const
{ return m_colors; }

SDL_Texture* TextureHandler::create_texture(std::string png_path) const
{
    // A texture has been created from this path already. 
    if(s_paths_to_textures.find(png_path) != s_paths_to_textures.end())
    {
        return s_paths_to_textures.at(png_path);
    }

    // If the file does not exist.
    if(!FileSystemHandler::does_directory_exist(png_path))
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("TextureHandler.create_texture()-> Path \""
            + png_path + "\" does not exist.", Frost::ERR);
        #endif

        exit(1);
    }

    SDL_Surface* temp_surface = IMG_Load(png_path.c_str());

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, temp_surface);

    SDL_FreeSurface(temp_surface);

    // Register this texture in the known textures that have been created.
    s_paths_to_textures.emplace(png_path, texture);

    return texture;
}

SDL_Texture* TextureHandler::create_font_atlas_texture(TTF_Font* font, int font_width, int font_height) const
{
    // Supported renderable characters of the engine.
    const std::string RENDERABLE_CHARACTERS = 
        "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    SDL_Texture* atlas_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, font_width * RENDERABLE_CHARACTERS.size(), font_height);

    // Save Render draw color.
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(m_renderer, &r, &g, &b, &a);  

    // Enable blending mode on the atlas texture
    SDL_SetTextureBlendMode(atlas_texture, SDL_BLENDMODE_BLEND);

    // Set the atlas texture as the render target
    SDL_SetRenderTarget(m_renderer, atlas_texture);

    // Set render draw color to fully transparent
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0); // Transparent background
    SDL_RenderClear(m_renderer);

    // Iterate through each renderable character, creating a glyph of each one and patching into 
    // the full atlas.
    for(int i = 0; i < RENDERABLE_CHARACTERS.size(); ++i)
    {
        // Render the glyph surface
        SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(font, RENDERABLE_CHARACTERS[i], 
            {255, 255, 255, 255});

        // Create a texture from the surface.
        SDL_Texture* glyphTexture = SDL_CreateTextureFromSurface(m_renderer, glyphSurface);
        SDL_FreeSurface(glyphSurface); // Free the surface after creating the texture

        // Copy the glyph texture to its position in the atlas.
        SDL_Rect destRect = {i * font_width, 0, font_width, font_height};
        SDL_RenderCopy(m_renderer, glyphTexture, nullptr, &destRect);

        SDL_DestroyTexture(glyphTexture); // Clean up the glyph texture.
    }

    // Reset Renderer.
    SDL_SetRenderTarget(m_renderer, nullptr);
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);

    return atlas_texture;
}


// Private 

void TextureHandler::_get_colors_from_disk()
{
    // List of colors in json format.
    const json colors = JsonHandler::get(m_color_data_path);

    // Iterate through each color.
    for(const json& color : colors)
    {
        // Create a color, and register it in the map using its name as the key.
        m_colors[color.at(0)] = Color(color.at(1), color.at(2), color.at(3), color.at(0));
    }
}
