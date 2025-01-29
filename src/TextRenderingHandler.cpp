/**
 * @file TextRenderingHandler.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class implementation.
 * @version 0.2
 * @date 24-12-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cstring>

#include "TextRenderingHandler.hpp"
#include "TextFileHandler.hpp"
#include "FileSystemHandler.hpp"
#include "CrashOutputHandler.hpp"


// Constructors / Deconstructor

TextRenderingHandler::TextRenderingHandler()
{
    // Default constructor should only be called for a placeholder object.

    m_font_point_size = 0;
    m_font_width = 0;
    m_font_height = 0;
    m_tex_handler = nullptr;
    m_atlas_texture = nullptr;

    _fetch_available_fonts();
}

TextRenderingHandler::TextRenderingHandler(TextureHandler* texture_handler, 
    uint8_t font_point_size, std::string font_path)
{
    m_tex_handler = texture_handler;

    _fetch_available_fonts();

    set_font_path_and_size(font_point_size, font_path);
}

TextRenderingHandler::TextRenderingHandler(const TextRenderingHandler& source)
{
    m_tex_handler = source.m_tex_handler;
    m_font_path = source.m_font_path; 
    m_available_font_paths = source.m_available_font_paths;

    _init_font_dimensions_and_atlas();
}

TextRenderingHandler::TextRenderingHandler(TextRenderingHandler&& source)
{
    m_font_point_size = source.m_font_point_size;
    m_font_width = source.m_font_width;
    m_font_height = source.m_font_height;
    m_src = source.m_src;
    m_dest = source.m_dest;
    m_font_path = std::move(source.m_font_path);
    m_font_path = source.m_font_path; 
    m_tex_handler = source.m_tex_handler;
    m_atlas_texture = source.m_atlas_texture;

    source.m_atlas_texture = nullptr;
    source.m_tex_handler = nullptr;
}

TextRenderingHandler& TextRenderingHandler::operator=(TextRenderingHandler&& source)
{
    m_font_point_size = source.m_font_point_size;
    m_font_width = source.m_font_width;
    m_font_height = source.m_font_height;
    m_src = source.m_src;
    m_dest = source.m_dest;
    m_font_path = std::move(source.m_font_path);

    m_tex_handler = source.m_tex_handler;

    // Destroy original texture of this object before transferring the source's one.
    SDL_DestroyTexture(m_atlas_texture);
    m_atlas_texture = source.m_atlas_texture;

    source.m_atlas_texture = nullptr;
    source.m_tex_handler = nullptr;

    return *this;
}

TextRenderingHandler& TextRenderingHandler::operator=(const TextRenderingHandler& source)
{
    m_tex_handler = source.m_tex_handler;
    m_font_path = source.m_font_path; 
    m_available_font_paths = source.m_available_font_paths;

    // This object's atlas texture will be automatically destroyed in this function before being
    // replaced.
    _init_font_dimensions_and_atlas();

    return *this;
}

TextRenderingHandler::~TextRenderingHandler()
{
    if(m_atlas_texture) SDL_DestroyTexture(m_atlas_texture);
}


// Public

void TextRenderingHandler::add_ch(char c, uint16_t x, uint16_t y, std::string color)
{
    // Subtracting the ascii '!' here to calculate the "index" of the target character in the font
    // atlas. Since the atlas starts at '!' and progresses linearly through the ascii values, 
    // simply calculate the index by the ascii value of this char deducted by '!'. Then multiply
    // by the font size to get the pixel value in the font atlas.
    m_src.x = (c - '!') * m_font_width;

    m_dest.x = x;
    m_dest.y = y;

    m_tex_handler->draw(m_atlas_texture, m_src, m_dest, color);
}

void TextRenderingHandler::set_font_size(uint8_t new_font_point_size)
{
    if(new_font_point_size < 11) new_font_point_size = 11;

    m_font_point_size = new_font_point_size;

    // Update the font dimensions and atlas since it has now changed.
    _init_font_dimensions_and_atlas();
}

void TextRenderingHandler::set_font_path(std::string new_font_path)
{
    m_font_path = new_font_path;

    // Update the font dimensions and atlas since it has now changed.
    _init_font_dimensions_and_atlas();
}

void TextRenderingHandler::set_font_path_and_size(uint8_t new_font_point_size, 
    std::string new_font_path)
{
    if(new_font_point_size < 11) new_font_point_size = 11;

    m_font_point_size = new_font_point_size;

    m_font_path = new_font_path;

    // Update the font dimensions and atlas since it has now changed.
    _init_font_dimensions_and_atlas();
}

uint8_t TextRenderingHandler::get_font_point_size() const { return m_font_point_size; }

uint8_t TextRenderingHandler::get_font_width() const { return m_font_width; }

uint8_t TextRenderingHandler::get_font_height() const { return m_font_height; }

const std::string& TextRenderingHandler::get_current_font_path() const
{ return m_font_path; }

const std::vector<std::string>& TextRenderingHandler::get_available_font_paths() const
    { return m_available_font_paths; }


// Private

void TextRenderingHandler::_fetch_available_fonts()
{
    // Fetch the fonts at the fonts directory.
    m_available_font_paths = FileSystemHandler::get_files_at_directory(m_fonts_directory);

    // If no fonts were found.
    if(m_available_font_paths.size() == 0)
    {
        OUTPUT_CRASH_DETAILS(" -> No fonts were found at the fonts directory: '" + m_fonts_directory + "'.\n");
        exit(1);
    }
}

void TextRenderingHandler::_init_font_dimensions_and_atlas()
{
    if(!FileSystemHandler::does_directory_exist(m_font_path))
    {
        OUTPUT_CRASH_DETAILS(" where 'm_font_path' = '" + m_font_path + "' -> Font path is invalid.\n");
        exit(1);
    }

    // Create font object from the font file.
    TTF_Font* font = TTF_OpenFont(m_font_path.c_str(), m_font_point_size);

    if(!font)
    {
        OUTPUT_CRASH_DETAILS(" -> SDL_TTF failed to create font object.\n");
        exit(1);
    }

    // Temp variables to grab the font dimensions from SDL. Using temp integers to later store in
    // uint8_ts for efficiency. Sure, it's negligible but it makes me feel good about memory 
    // efficiency.
    int text_width, text_height;

    // Get the size in pixels of a single character. Since the engine only supports Monospaced 
    // fonts, the size of one character represents the size of them all.
    TTF_SizeUTF8(font, RENDERABLE_CHARACTERS, &text_width, &text_height);

    m_font_width = text_width / std::strlen(RENDERABLE_CHARACTERS);
    m_font_height = text_height;

    m_src.y = 0;
    m_src.w = m_font_width;
    m_src.h = m_font_height;

    m_dest.w = m_font_width;
    m_dest.h = m_font_height;

    // If a font texture already exists, destroy it as it will be replaced.
    if(m_atlas_texture)
    {
        SDL_DestroyTexture(m_atlas_texture);
    }

    m_atlas_texture = m_tex_handler->create_font_atlas_texture(font);

    TTF_CloseFont(font);
}

