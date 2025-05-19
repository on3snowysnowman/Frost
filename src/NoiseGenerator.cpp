/**
 * @file NoiseGenerator.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 05-08-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "NoiseGenerator.hpp"


// Constructors / Deconstructor

NoiseGenerator::NoiseGenerator() 
{
    set_offset(0, 0);
}


// Public

void NoiseGenerator::set_offset(float x_offset, float y_offset)
{
    m_x_offset = x_offset;
    m_y_offset = y_offset;
}

float NoiseGenerator::get_noise_at_position(float x, float y) const
    { return m_noise_generator.GetNoise(x + m_x_offset, y + m_y_offset); }

std::vector<float> NoiseGenerator::generate_full_noise_map(uint16_t width, 
    uint16_t height)
{
    std::vector<float> noise_map;

    noise_map.reserve(width * height);

    for(uint16_t row = 0; row < height; ++row)
    {
        for(uint16_t col = 0; col < width; ++col)
        {
            noise_map.push_back(get_noise_at_position(col + m_x_offset, 
                row + m_y_offset));
        }
    }

    return noise_map;
}

std::vector<float> NoiseGenerator::condense_noise_map(
    const std::vector<float>& noise_map, uint16_t old_width, 
    uint16_t old_height, uint16_t new_width, uint16_t new_height) const
{
    std::vector<float> condensed_map;
    condensed_map.reserve(new_width * new_height);

    uint16_t grid_width = old_width / new_width;
    uint16_t grid_height = old_height / new_height;

    for(int row = 0; row < new_height; ++row)
    {
        for(int col = 0; col < new_width; ++col)
        {
            condensed_map.push_back(_get_average_grid_value
            (
                noise_map, 
                (row * grid_height * old_width) + (col * grid_width), 
                grid_width, grid_height, old_width - grid_width)
            );
        }
    }

    return condensed_map;
}

std::vector<SDL_Color> NoiseGenerator::colorize_noise_map(
    const std::vector<float>& noise_map,
    const std::vector<std::pair<float, SDL_Color>>& color_assignments) const
{
    if(color_assignments.size() == 0) return {};

    std::vector<SDL_Color> colored_noise;
    colored_noise.reserve(noise_map.size());

    // Default to the last color in case the user does not properly specify a
    // color range for all noise values.
    SDL_Color targ_color = color_assignments.back().second;

    for(uint32_t i = 0; i < noise_map.size(); ++i)
    {
        float grayscale_noise_value = noise_map.at(i);

        // Iterate through each color assignment, and see if the parsed 
        // grayscale value is less than or equal to the assignment's value. 
        for(const std::pair<float, SDL_Color>& _pair : color_assignments)
        {
            if(grayscale_noise_value <= _pair.first)
            {
                targ_color = _pair.second;
                break;
            } 
        }

        colored_noise.push_back(targ_color);
    }

    return colored_noise;
}

// Private

float NoiseGenerator::_get_average_grid_value(
    const std::vector<float>& noise_map, uint32_t start_index, 
    uint16_t grid_width, uint16_t grid_height, uint16_t jump_interval) const
{
    float sum = 0;

    for(uint32_t row = 0; row < grid_height; ++row)
    {
        for(uint32_t col = 0; col < grid_width; ++col)
        {
            sum += noise_map.at(start_index);
            ++start_index;
        }

        start_index += jump_interval;
    }

    // Return the average value of this grid by dividing the sum by the Area.
    return sum / (grid_width * grid_height);
}

