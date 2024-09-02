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
