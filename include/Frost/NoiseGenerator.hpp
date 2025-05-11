/**
 * @file NoiseGenerator.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 05-08-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <SDL_pixels.h>

#include "FastNoiseLite.h"


/**
 * @brief Provides functionality for generating and modifying fluid noise maps.
 * 
 * This class acts as a wrapper around the 'FastNoiseLite' library, which 
 * handles the core noise generation. Additional methods are provided to 
 * manipulate the generated noise, such as condensing or coloring the maps.
 */
class NoiseGenerator
{

public:

    NoiseGenerator();

    void set_offset(float x_offset, float y_offset);

    /**
     * @brief Generates and retrieves a noise value for the specified coordinates.
     * 
     * This method uses the internal noise generator to compute a noise value
     * based on the given X and Y coordinates.
     * 
     * @param x The X coordinate for noise generation.
     * @param y The Y coordinate for noise generation.
     * @return float The generated noise value.
     */
    float get_noise_at_position(float x, float y) const;

    std::vector<float> generate_full_noise_map(uint16_t width, 
        uint16_t height);

    std::vector<float> condense_noise_map(
        const std::vector<float>& noise_map, uint16_t old_width, 
        uint16_t old_height, uint16_t new_width, uint16_t new_height) const;

    std::vector<SDL_Color> colorize_noise_map(
        const std::vector<float>& noise_map,
        const std::vector<
            std::pair<float, SDL_Color>>& color_assignments) const;

    /**
     * @brief Accesses the underlying noise generator for customization.
     * 
     * This method provides direct access to the internal FastNoiseLite 
     * instance, allowing users to configure specific parameters for noise 
     * generation as needed.
     * 
     * @return FastNoiseLite& A reference to the internal noise generator.
     */
    FastNoiseLite& get_noise_generator() { return m_noise_generator; }

private:

    // Members

    float m_x_offset;
    float m_y_offset;

    FastNoiseLite m_noise_generator;


    // Methods 

    float _get_average_grid_value(const std::vector<float>& noise_map,
        uint32_t start_index, uint16_t grid_width, uint16_t grid_height,
        uint16_t jump_interval) const;
};
