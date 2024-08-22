/**
 * @file Tile.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for EntityManager.
 * @version 0.1
 * @date 2024-08-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <cstdint>
#include <queue>
#include <list>

#include "Component.hpp"


typedef uint64_t entity_id;


/** @brief Static class that manages Entitys, their components and scripts. */
class EntityManager
{

public:



private:

    // IDs of entities that have been deleted, and their ID can be recycled.
    static std::queue<entity_id> available_entity_ids;

    /** All components, each index of the outer vector resembles the Component ID, whereas the
     * vector at each index contains all Components of that ID.
     */
    static std::vector<std::vector<Component*>> components;
};
