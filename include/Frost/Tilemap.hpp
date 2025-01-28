/**
 * @file Tilemap.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 01-17-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <unordered_map>
#include <queue>
#include <list>

#include "ExceptionOutputHandler.hpp"

using entity_id = uint64_t;


template<typename T, uint64_t N>
class Tilemap
{

private:

    // These private members are placed at the top of the class so that they are made available 
    // to the components during compile time.

    // Classes / Structs

    struct EntityTracker
    {
        bool is_entity_active;
        uint16_t x_position;
        uint16_t y_position;
        T entity;
    };

public:

    using content_type = std::array<std::array<std::list<T>, N>, N>;

    Tilemap() {}

    /** Adds the entity with the passed ID to a position in the Tilemap.
     * 
     * @param ID ID of the entity to move.
     * @param x New x position.
     * @param y New y position.
     */
    void add_entity_to_tilemap(const entity_id ID, uint16_t x, uint16_t y)
    {
        try
        {
            contents.at(y).at(x).push_back(all_entities.at(ID).entity);
        }

        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            if(!_does_id_exist(ID))
            {
                error_message = "Invalid entity ID: " + std::to_string(ID);
            }

            else if(x >= N)
            {
                error_message = "Invalid x position: " + std::to_string(x);
            }

            else if(y >= N)
            {
                error_message = "Invalid y position: " + std::to_string(y);
            }

            OUTPUT_EXCEPTION(error_message);
            exit(1);
        }
    }


    /** Moves the entity with the passed ID to a new position. 
     * 
     * @param ID ID of the entity to move.
     * @param x New x position.
     * @param y New y position.
    */
    void move_entity(const entity_id ID, uint16_t x, uint16_t y)
    {
        // EntityTracker* targ_tracker;

        // try
        // {
        //     targ_tracker = &all_entities.at(ID);
        // }

        // catch(const std::out_of_range& e)
        // {
        //     std::string error_message = e.what();

        //     if(!_does_id_exist(ID))
        //     {
        //         error_message = "Invalid entity ID: " + std::to_string(ID);
        //     }

        //     OUTPUT_EXCEPTION(error_message);
        //     exit(1);
        // }

        remove_entity_from_tilemap(ID);
        add_entity_to_tilemap(ID, x, y);
    }

    void modify_entity_position(const entity_id ID, int16_t x_delta, int16_t y_delta)
    {

    }

    void remove_entity_from_tilemap(const entity_id ID)
    {

    }

    void free_entity(const entity_id ID)
    {
        EntityTracker* targ_tracker;

        try
        {
            targ_tracker = &all_entities.at(ID);
        }

        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            if(!_does_id_exist(ID))
            {
                error_message = "Invalid entity ID: " + std::to_string(ID);
            }

            OUTPUT_EXCEPTION(error_message);
            exit(1);
        }
        
        if(_is_id_in_tilemap(ID))
        {
            // If the entity is in the Tilemap, remove it before it is freed.
            _remove_id_from_contents(ID);
        }

        // Flag this entity as deactivated.
        targ_tracker->is_entity_active = false;

        available_ids.push(ID);
    }

    entity_id register_entity(const T& entity)
    {
        entity_id id;

        // If a recycled ID is available.
        if(available_ids.size() > 0)
        {
            id = available_ids.front();
            available_ids.pop();

            all_entities.at(id).entity = T(entity);
        }

        // No IDs are available, create a new one.
        else 
        {
            id = all_entities.size();   
            all_entities.push_back(EntityTracker{true, N, N, entity});
        }

        return id;
    }

    /** 
     * @brief Returns the size of a single dimension of the Tilemap. 
     * 
     * @note This is only the size of one "side" of the Tilemap. If the total size of the Tilemap 
     * needs to be found, square this result to get the dimension * dimension size. */
    uint64_t tilemap_size() const { return N; }

    const content_type& get_contents() const { return contents; }

    const std::list<T>& get_entities_at_position(const entity_id ID, uint16_t x,
        uint16_t y) const
    {
        if(x >= N || y >= N)
        {
            // TextFileHandler::add_to_buffer("Tilemap::get_entities_at_position(const entity_id ID, "
            //     "uint16_t x, uint16_t y) where 'x' = " + std::to_string(x) + " & 'y' = '" + 
            //     std::to_string(y) + "' -> Attempted to retrieve list of entities from invalid "
            //     "position.\n");
            // TextFileHandler::write("CrashLog.txt");
            exit(1);
        }

        return contents.at(y).at(x);
    }

    const EntityTracker& get_entity_data(const entity_id ID) const 
    { 
        if(!_does_id_exist(ID))
        {
            
        }

        else if(_is_entity_active(ID))
        {

        }

        return all_entities.at(ID); 
    }

private:

    // Members

    /** Underlying contents of the Tilemap that stores all added entities. */
    content_type contents;

    // Ids that have been decomissioned and can be recycled.
    std::queue<entity_id> available_ids;
 
    /** Vector of all registered entities. The index of the vector represents the id of the object
     * that is registered. */
    std::vector<EntityTracker> all_entities; 


    // Methods

    void _remove_id_from_contents(const entity_id ID)
    {
        const EntityTracker& targ_tracker = all_entities.at(ID);

        // Set both coordinate positions to the size of the Tilemap's single dimension, effectively
        // 'removing' them from the Tilemap.
        targ_tracker.x_position = N;
        targ_tracker.y_position = N;

        // Iterator to the beginning of the target list at the entity's position.
        typename std::list<T*>::const_iterator it_begin = 
            contents.at(targ_tracker.y_position).at(targ_tracker.x_position).begin();

        // Iterator to the end of the target list at the entity's position.
        typename std::list<T*>::const_iterator it_end = 
            contents.at(targ_tracker.y_position).at(targ_tracker.x_position).end();

        while(it_begin != it_end)
        {
            if(*it_begin == targ_tracker.entity)
            {
                contents.at(targ_tracker.y_position).at(targ_tracker.x_position).erase(it_begin);
                return;
            }

            ++it_begin;
        }

        // If this part of the function is reached, the entity was not found. Output an error and 
        // softly crash the program.
    }

    // Returns true if the passed ID exists.
    bool _does_id_exist(const entity_id ID) { return ID < all_entities.size(); }

    /** Returns true if the passed ID is present in the tilemap. Since this is an internal method,
     * it is assumed that the ID is valid. */
    bool _is_id_in_tilemap(const entity_id ID) { return all_entities.at(ID).x_position < N; }

    /**
     * Returns true if the passed ID is registered to an active entity, and not an ID that has been
     * freed. */
    bool _is_entity_active(const entity_id ID)
    {
        return all_entities.at(ID).entity;
    }
};

