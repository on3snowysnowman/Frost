/**
 * @file Point.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 05-17-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <cstdint>
#include <functional>

/**
 * @brief A 2D point in space.
 */
struct Point2D
{
    uint32_t x;
    uint32_t y;

    bool operator==(const Point2D& other) const 
        { return x == other.x && y == other.y; }
};

struct Point2DHasher {
    std::size_t operator()(const Point2D& p) const {
        
        return std::hash<uint32_t>{}(p.x) ^ (std::hash<uint32_t>{}(p.y) << 1);
    }
};
