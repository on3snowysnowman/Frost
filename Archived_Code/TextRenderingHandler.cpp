#include <SDL_keycode.h>
#include <cmath>

#include "TextRenderingHandler.hpp"
#include "JsonHandler.hpp"
#include "Fr_Math.hpp"


// Constructors / Deconstructor

TextRenderingHandler::TextRenderingHandler() 
{
    m_font_texture = nullptr;
    m_texture_handler = nullptr;
}

TextRenderingHandler::TextRenderingHandler(TextureHandler* texture_handler)
{
    m_texture_handler = texture_handler;

    // Register the font from the disk.
    _load_font_from_disk();
}


// Public

void TextRenderingHandler::render() 
{
    // While there are queued characters
    while(!m_queued_characters.empty())
    {
        const QueuedCharacter& q_char = m_queued_characters.front();
        m_queued_characters.pop();

        // Draw the character on the screen. 
        m_texture_handler->draw(m_font_texture, q_char.source, q_char.dest, q_char.color);
    }
}

void TextRenderingHandler::clear_buffered_content() 
{
    while(!m_queued_characters.empty()) m_queued_characters.pop();
}

void TextRenderingHandler::increase_size_scale(float amount) 
{ 
    m_size_scale_factor += amount;

    m_size_scale_factor = 
        Frost::clamp_float_to_maximum(m_size_scale_factor, MAXIMUM_SIZE_SCALE_FACTOR);
}

void TextRenderingHandler::set_size_scale(float amount)
{
    amount = std::abs(amount);

    m_size_scale_factor = Frost::clamp_float_to_maximum(amount, MAXIMUM_SIZE_SCALE_FACTOR);

}

void TextRenderingHandler::decrease_size_scale(float amount) 
{
    m_size_scale_factor -= amount;

    m_size_scale_factor = Frost::clamp_float_to_minimum(m_size_scale_factor, 1.0f);
}

void TextRenderingHandler::draw_character_now(char c, uint16_t x, uint16_t y, std::string color)
{
    SDL_Rect source, dest;

    source.w = m_font_width;
    source.h = m_font_height;
    source.x = m_char_source_positions.at(c).first; 
    source.y = m_char_source_positions.at(c).second; 

    dest.w = get_scaled_font_width();
    dest.h = get_scaled_font_height();
    dest.x = x;
    dest.y = y;

    m_texture_handler->draw(m_font_texture, source, dest, color);
}

void TextRenderingHandler::add_ch(char c, uint16_t x, uint16_t y, std::string color) 
{
    QueuedCharacter character_to_render;

    // Set the data for the queued character object.

    character_to_render.symbol = c;

    character_to_render.color = color;

    character_to_render.source.w = m_font_width;
    character_to_render.source.h = m_font_height;
    character_to_render.source.x = m_char_source_positions.at(c).first; 
    character_to_render.source.y = m_char_source_positions.at(c).second; 

    character_to_render.dest.w = get_scaled_font_width();
    character_to_render.dest.h = get_scaled_font_height();
    character_to_render.dest.x = x;
    character_to_render.dest.y = y;

    // Add this queued character object to the queue of characters to be rendered.
    m_queued_characters.push(std::move(character_to_render));
}

float TextRenderingHandler::get_size_scale() const { return m_size_scale_factor; }

uint16_t TextRenderingHandler::get_scaled_font_width() const 
{ return _get_scaled_value(m_font_width); }

uint16_t TextRenderingHandler::get_scaled_font_height() const
{ return _get_scaled_value(m_font_height); }


// Private
#include <iostream>
void TextRenderingHandler::_load_font_from_disk()
{
    // Contains the path to the font png, along with the positions of each character in the png.
    const json font_data = JsonHandler::get(m_font_data_path);

    // Create the texture of the font.
    m_font_texture = m_texture_handler->create_texture(font_data.at("png_path"));

    m_font_width = font_data.at("font_width");
    m_font_height = font_data.at("font_height");

    // Iterate through each character container in all the characters.
    for(const json& char_data : font_data.at("character_data"))
    {
        // Get the data for this specific character from the json file, and register it into the 
        // map. Casting to a uint8_t here instead of char is required for casting the return value
        // nlohmann::json. nlohmann::json does not support casting to a char directly. 

        m_char_source_positions[uint8_t(char_data.at(0))] = 
            std::make_pair(char_data.at(1), char_data.at(2));
    }
}

uint16_t TextRenderingHandler::_get_scaled_value(uint16_t value) const 
{ return std::floor(value * m_size_scale_factor); }

