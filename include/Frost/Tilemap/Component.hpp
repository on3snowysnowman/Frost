/**
 * @file Component.hpp
 * @author Joel Height (On3sSnowySnowman@gmail.com)
 * @brief Declaration for Component.hpp 
 * @version 0.1
 * @date 2024-08-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <cstdint>


typedef uint64_t component_id;


namespace
{
    /** When this method is called for the first time by a new Component type, a new ID will be 
     * generated. Further calls will return a constant ID for each respective Component type.
     */
    component_id get_next_id() 
    {
        static component_id current_id = 0;

        return current_id++;
    }
}


/** Returns the id of the passed Component type. If this is a new Component type that hasn't called 
 * this method yet, a new id will be generated. 
 * 
 * @param T Type of the Component. */
template<typename T>
component_id get_component_id() 
{
    static component_id id = get_next_id();

    return id;
}


/** @brief Base class for individual Components to inherit. */
class Component {};

