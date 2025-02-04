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
#include "TextFileHandler.hpp"
#include "TextRenderingHandler.hpp"
#include "CrashOutputHandler.hpp"

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

TextureHandler::TextureHandler(SDL_Renderer* renderer)
{
    m_renderer = renderer;
}

TextureHandler::~TextureHandler()
{
    for(std::pair<SDL_Texture*, std::string> _pair : s_textures_to_paths)
    {
        SDL_DestroyTexture(_pair.first);
    }
}


// Public

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


// const std::unordered_map<std::string, Frost::Color>& TextureHandler::get_colors() const
// { return m_colors; }

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
        OUTPUT_CRASH_DETAILS(" where 'png_path' = '" + png_path + "' -> Path does not exist.\n");
        exit(1);
    }

    SDL_Surface* temp_surface = IMG_Load(png_path.c_str());

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, temp_surface);

    SDL_FreeSurface(temp_surface);

    // Register this texture in the known textures that have been created.
    s_paths_to_textures.emplace(png_path, texture);

    return texture;
}

SDL_Texture* TextureHandler::create_font_atlas_texture(std::string font_path,
    int font_point_size, uint16_t& font_width, uint16_t& font_height)
{
    // Create font object from the font file.
    TTF_Font* font = TTF_OpenFont(font_path.c_str(), font_point_size);

    // SDL Failed to create font.
    if(!font)
    {
        OUTPUT_CRASH_DETAILS(" -> SDL_TTF failed to create font object: " + 
            std::string(SDL_GetError()) + ".\n");
        exit(1);
    }

    // Fetch font dimensions
    _calculate_font_dimensions(font, font_width, font_height);

    // Full atlas to contain all rendered glyphs.
    SDL_Texture* font_atlas = SDL_CreateTexture(m_renderer, 
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, font_width * 
        NUM_RENDERABLE_CHARS, font_height);

    // Change this texture to use blending, so that alpha channels are 
    // respected when rendering to the screen later. 
    SDL_SetTextureBlendMode(font_atlas, SDL_BLENDMODE_BLEND);

    // Dimensions for each individual glyph to be copied into the final atlas.
    SDL_Rect src {0, 0, font_width, font_height};
    SDL_Rect dest {0, 0, font_width, font_height};

    // Color of the each glyph. 
    static constexpr SDL_Color WHITE {255, 255, 255, 255};

    // Set the font atlas as the rendering target.
    SDL_SetRenderTarget(m_renderer, font_atlas);

    // Iterate through each renderable character and draw its glyph to the 
    // final texture.
    for(uint8_t i = 0; i < NUM_RENDERABLE_CHARS; ++i)
    {
        // Create a const char* of the target char to render. Stored in a new 
        // buffer like this, since you can't pass a single char to SDL's TTF 
        // render method. Only a const char* with a null terminated character.
        char targ_char[2] = {RENDERABLE_CHARACTERS[i], '\0'};

        SDL_Surface* glyph_surface = TTF_RenderUTF8_Blended(font, 
            targ_char, WHITE);

        // Convert the glyph surface into a texture. 
        SDL_Texture* glyph_texture = SDL_CreateTextureFromSurface(m_renderer, 
            glyph_surface);

        // Set the blend mode to NONE, so that when the glyph is copied into
        // the font atlas, it does not blend it with any values. Simply 
        // replaces the values of the font atlas with the values of the glyph.
        SDL_SetTextureBlendMode(glyph_texture, SDL_BLENDMODE_NONE);

        // X pixel position to place the glyph inside the font atlas. 
        dest.x = i * font_width;
        
        // Copy this glyph into the render target which is set to the atlas
        // texture.
        SDL_RenderCopy(m_renderer, glyph_texture, &src, &dest);

        // Cleanup.
        SDL_FreeSurface(glyph_surface);
        SDL_DestroyTexture(glyph_texture);
    }

    // Restore the render target to the screen. 
    SDL_SetRenderTarget(m_renderer, nullptr);

    // Cleanup.
    TTF_CloseFont(font);

    return font_atlas;
}


// Private 

void TextureHandler::_calculate_font_dimensions(TTF_Font* font, 
    uint16_t& font_width, uint16_t& font_height)
{
    font_width = font_height = 0;

    // Temp variables for passing references to SDL_TTF calculations, to then 
    // be placed into the smaller uin16_t actual font variables.
    int temp_width, temp_height;

    // Calculate dimension for all rendered characters in a line, to account 
    // for any sticklers like 'j' or 'g' that may be greater in vertical glyph
    // height than characters like 'A". The final height will be equal to the
    // largest vertical glyph, so the largest case is always accounted for.
    TTF_SizeUTF8(font, RENDERABLE_CHARACTERS, &temp_width, &temp_height);

    // Divide the SDL calculated width by the number of renderable characters,
    // since the width is the size of all rendered characters in a line. 
    font_width = temp_width / NUM_RENDERABLE_CHARS;
    font_height = temp_height;
}
