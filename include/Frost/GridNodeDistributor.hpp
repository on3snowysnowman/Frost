/**
 * @file GridNodeDistributor.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 05-17-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <vector>

#include "Fr_Random.hpp"
#include "Point.hpp"


class GridNodeDistributor
{

public:

    GridNodeDistributor();

    std::vector<Point2D> get_single_value_node_distribution(
        uint32_t grid_width, uint32_t grid_height, uint32_t chunk_width,
        uint32_t chunk_height, uint32_t nodes_per_chunk, 
        uint8_t extra_node_chance = 0, uint32_t extra_node_threshold = 0);

private:

    void _generate_nodes_in_chunk(std::vector<Point2D>& all_placed_nodes, 
        uint32_t chunk_start_x, uint32_t chunk_start_y, uint32_t chunk_width, 
        uint32_t chunk_height, uint32_t nodes_per_chunk);
};

