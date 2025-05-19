/**
 * @file GridNodeDistributor.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 05-17-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "GridNodeDistributor.hpp"

#include <unordered_set>
#include <iostream>


// Constructors / Deconstructor

GridNodeDistributor::GridNodeDistributor() {}


// Public

std::vector<Point2D> GridNodeDistributor::get_single_value_node_distribution(
    uint32_t grid_width, uint32_t grid_height, uint32_t chunk_width,
    uint32_t chunk_height, uint32_t nodes_per_chunk, uint8_t extra_node_chance,
    uint32_t extra_node_threshold)
{
    // Chunk dimensions are not whole factors of the total grid dimensions.
    if(grid_width % chunk_width != 0 ||
        grid_height % chunk_height != 0) return {};

    std::vector<Point2D> placed_nodes;

    uint32_t num_extra_nodes = 0;

    // Calculate number of columns and rows for the chunks.
    int num_chunk_col = grid_width / chunk_width;
    int num_chunk_row = grid_height / chunk_height;

    // Iterate over each chunk
    for(int row = 0; row < num_chunk_row; ++row)
    {
        for(int col = 0; col < num_chunk_col; ++col)
        {
            if(extra_node_chance > 0 && 
                FrostRandom::get_random_num<uint8_t>(1, 100)
                <= extra_node_chance)
            {
                num_extra_nodes = FrostRandom::get_random_num<uint32_t>(1, 
                    extra_node_threshold);
            }

            // Generate the nodes for this chunk.
            _generate_nodes_in_chunk(placed_nodes, col * chunk_width, 
                row * chunk_height, chunk_width, chunk_height, nodes_per_chunk
                + num_extra_nodes);

            num_extra_nodes = 0;
        }
    }

    return placed_nodes;
}


// Private

void GridNodeDistributor::_generate_nodes_in_chunk(
    std::vector<Point2D>& all_placed_nodes, uint32_t chunk_start_x, 
    uint32_t chunk_start_y, uint32_t chunk_width, uint32_t chunk_height, 
    uint32_t nodes_per_chunk)
{
    // #TODO: For the first node, place it in a random position in the grid,
    // since it's known the grid is empty and it can go anywhere.

    std::unordered_set<Point2D, Point2DHasher> occupied_positions;

    uint32_t chunk_area = chunk_width * chunk_height;

    uint32_t num_placed_nodes = 0;

    uint32_t row = 0;

    // Starting at the top left of the grid, continues iterates through each 
    // position of the grid until the target number of nodes have been added, 
    // unless there are no more positions available to place a node.
    while(true)
    {
        // Iterate through each column in this row.
        for(uint32_t col = 0; col < chunk_width; ++col)
        {
            // This position has already been taken by a Node, skip it.
            if(occupied_positions.find({col, row}) != occupied_positions.end())
                continue;

            // If the generation chance failed, skip this position. The 
            // upper bound is the area - the number of positions already taken 
            // here, so that each node in the grid has an equal chance to 
            // spawn a node as the rest of the open spots.
            if(FrostRandom::get_random_num<uint32_t>(1, 
                chunk_area - occupied_positions.size()) > 1) continue;

            // Add this chunk position to the set of occupied positions.
            occupied_positions.emplace(Point2D{col, row});

            // If the number of occupied positions matches the number of nodes
            // per chunk, there are no more available positions to place any 
            // more nodes.
            if(occupied_positions.size() == chunk_area) return;

            // This position is not occupied, and the generation chance 
            // succeeded. Place a node here.
            
            Point2D node {col + chunk_start_x, row + chunk_start_y};

            // Add this node to the tracked placed nodes.
            all_placed_nodes.push_back(node);

            // We've reached the target number of nodes to generate in this
            // chunk.
            if(++num_placed_nodes == nodes_per_chunk) return;
        }

        // If the row has reached the bottom of the grid, place it back at
        // the top to keep searching for node positions.
        if(++row >= chunk_height) row = 0;
    }
}
