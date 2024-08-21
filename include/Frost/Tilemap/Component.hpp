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

    component_id get_next_id() 
    {
        static component_id current_id = 0;

        return current_id++;
    }

}



template<typename T>
class Component
{

public:
    
    Component() : m_ID(get_component_id_from_type()) {}

    /** Returns the id type of this Component. */
    component_id get_id() const { return m_ID; }

private:

    // Members

    /** Unique ID of this Component type. Different derived Component classes will have different 
     * IDs. */
    const component_id m_ID;


    // Methods

    /** Returns the id of this Component type. If this is a new Component type that hasn't called 
    * this method yet, a new id will be generated. */
    component_id get_component_id_from_type()
    {
        static component_id id = get_next_id();

        return id;
    }
};

