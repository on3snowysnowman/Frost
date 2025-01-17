/**
 * @file TextRenderingHandler.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class definition.
 * @version 0.2
 * @date 24-12-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <string>
#include <vector>

#include <SDL_ttf.h>
#include <SDL_image.h>

#include "TextureHandler.hpp"


class TextRenderingHandler
{

public:

    TextRenderingHandler();

    TextRenderingHandler(TextureHandler* texture_handler, uint8_t font_point_size = 24);

    TextRenderingHandler(const TextRenderingHandler& source);

    TextRenderingHandler(TextRenderingHandler&& source);

    TextRenderingHandler& operator=(TextRenderingHandler&& source);

    TextRenderingHandler& operator=(const TextRenderingHandler& source);

    ~TextRenderingHandler();

    /** Adds a char to the screen at a pixel position. Color is supported. */
    void add_ch(char c, uint16_t x, uint16_t y, std::string color = "White");

    /** Sets the font point size to a new value. 5 is the minimum. */
    void set_font_size(uint8_t new_font_point_size);

    /** Sets the font path to a new path, changing the font. */
    void set_font_path(std::string new_font_path);

    /** Returns the font point size. */
    uint8_t get_font_point_size() const;

    /** Returns the font's width.*/
    uint8_t get_font_width() const;

    /** Returns the font's height.*/
    uint8_t get_font_height() const;

    /** Returns the available font paths. */
    const std::vector<std::string>& get_available_font_paths() const;

private:

    // Members

    // Pint size of the font. 
    uint8_t m_font_point_size;

    uint8_t m_font_width; // Width of the active font.
    uint8_t m_font_height; // Height of the active font.

    SDL_Rect m_src; // Dimensions to splice from the font atlas when rendering a character.
    SDL_Rect m_dest; // Dimensions to place character on screen when rendering a character.

    std::string m_fonts_directory = "data/fonts";

    std::string m_font_path; // Path to the active font.

    // List of available fonts found at the data/fonts path. All files are assumed to be valid,
    // monospaced fonts.
    std::vector<std::string> m_available_font_paths;

    TextureHandler* m_tex_handler; 

    SDL_Texture* m_atlas_texture = nullptr; // Full texture containing all renderable characters.

    // Methods

    /** Iterates through the fonts directory to fetch and track each available font file. */
    void _fetch_available_fonts();

    /** 
     * Loads font from disk, establishing dimensions and creating an atlas texture to splice from
     * for character rendering. */
    void _init_font_dimensions_and_atlas();
};
