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

#include "CrashOutputHandler.hpp"

using entity_id = uint64_t;


template<typename T, uint64_t N>
class Tilemap
{

public:

    // Classes / Structs

    struct EntityTracker
    {
        bool is_entity_active;
        uint16_t x_position;
        uint16_t y_position;
        T entity;
    };

    using content_type = std::array<std::array<std::list<T>, N>, N>;

    Tilemap() {}

    /** Adds the entity with the passed ID to a position in the Tilemap.
     * 
     * @note There are no checks in this function for if the entity already exists in the Tilemap.
     * The entity will simply be added at the passed coordinates, and if it already exists there 
     * could be undefined behavior. If the user is unsure, use the `is_entity_in_tilemap` method.
     * 
     * @param id ID of the entity to move.
     * @param x New x position.
     * @param y New y position.
     */
    void add_entity_to_tilemap(const entity_id id, uint16_t x, uint16_t y)
    {
        EntityTracker* targ_tracker {};
        
        try
        {
            // Get the tracker assigned to this entity.
            targ_tracker = &all_entities.at(id);

            // Add the entity to the passed coordinate position.
            contents.at(y).at(x).push_back(targ_tracker->entity);

        }

        // If an error ocurred when placing the entity, handle it.
        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            // If the entity's ID is not a valid one.
            if(!_does_id_exist(id))
            {
                error_message = "Invalid entity ID: " + std::to_string(id);
            }

            // The x position is out of bounds.
            else if(x >= N)
            {
                error_message = "Invalid x position: " + std::to_string(x);
            }

            // The y position is out of bounds.
            else if(y >= N)
            {
                error_message = "Invalid y position: " + std::to_string(y);
            }

            // Output the exception to the crash log file and softly crash.
            OUTPUT_CRASH_DETAILS(error_message);
            exit(1);
        }

        // Update the tracker's position.
        targ_tracker->x_position = x;
        targ_tracker->y_position = y;
    }

    /** Moves the entity with the passed ID to a new position. 
     * 
     * @param id ID of the entity to move.
     * @param x New x position.
     * @param y New y position.
    */
    void move_entity(const entity_id id, uint16_t x, uint16_t y)
    {
        _remove_entity_from_tilemap(id);
        add_entity_to_tilemap(id, x, y);
    }

    /**
     * @brief Modifies the entity with the passed ID's position by the delta amount in each 
     * dimension.
     * 
     * The delta for both coordinates are simply added to the entity's present position, so passing
     * a negative number through the coordinate deltas will make the entity move left, or up 
     * respectfully.
     * 
     * There is automatic bounds checking for the new position for the entity. This method can
     * be safely called with any delta value and the position will be checked.
     * 
     * @param id ID of the entity to modify.
     * @param x_delta Amount to add to the x position.
     * @param y_delta Amount to add to the y position. 
     */
    void modify_entity_position(const entity_id id, int16_t x_delta, int16_t y_delta)
    {
        EntityTracker* targ_tracker {};

        try
        {
            // Get the tracker assigned to this entity.
            targ_tracker = &all_entities.at(id);
        }
        
        // If an error ocurred when placing the entity, handle it.
        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            // If the entity's ID is not a valid one.
            if(!_does_id_exist(id))
            {
                error_message = "Invalid entity ID: " + std::to_string(id);
            }

            // The x position is out of bounds.
            else if(targ_tracker->x_position >= N)
            {
                error_message = "Invalid x position: " + std::to_string(targ_tracker->x_position);
            }

            // The y position is out of bounds.
            else if(targ_tracker->y_position >= N)
            {
                error_message = "Invalid y position: " + std::to_string(targ_tracker->y_position);
            }

            // Output the exception to the crash log file and softly crash.
            OUTPUT_CRASH_DETAILS(error_message);
            exit(1);
        }

        // If the new position is out of bounds, do nothing.
        if(targ_tracker->x_position + x_delta >= N || targ_tracker->y_position + y_delta>= N) 
            return;

        _remove_entity_from_tilemap(id, targ_tracker->x_position, targ_tracker->y_position);

        // Update the tracker's components with the passed deltas.
        targ_tracker->x_position += x_delta;
        targ_tracker->y_position += y_delta; 

        // Add the entity at the updated tracker's position coordinates.
        add_entity_to_tilemap(id, targ_tracker->x_position, 
            targ_tracker->y_position);
    }

    /**
     * @brief Removes the entity with the passed ID from the tilemap. 
     * 
     * The position of the entity is tracked internally, so no coordinate parameters are needed.
     * 
     * @param id Id of th entity to remove.
     */
    void remove_entity_from_tilemap(const entity_id id)
    {
        _remove_entity_from_tilemap(id);
    
        // If the method above successfully executes, it is guaranteed that the passed ID exists. 

        // Set both coordinate positions to the size of the Tilemap's single dimension,
        // signalling that this ID is removed from the Tilemap.
        all_entities.at(id).x_position = N;
        all_entities.at(id).y_position = N;
    }

    /**
     * @brief Releases the internally tracked components that were tied to this entity, and allows 
     * the Tilemap to reuse the ID for another entity. 
     * 
     * Call this method when an entity is no longer being used with the Tilemap. Do not attempt to 
     * use this ID again unless it is given again when calling the `register_entity` method with 
     * another entity.  
     * 
     * @param id ID of the entity to free.
     */
    void free_entity(const entity_id id)
    {
        EntityTracker* targ_tracker;

        try
        {
            // Get the tracker assigned to this entity.
            targ_tracker = &all_entities.at(id);
        }

        // If an error ocurred when freeing the entity, handle it.
        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            // If the entity's ID is not a valid one.
            if(!_does_id_exist(id))
            {
                error_message = "Invalid entity ID: " + std::to_string(id);
            }

            OUTPUT_CRASH_DETAILS(error_message);
            exit(1);
        }
        
        if(_is_id_in_tilemap(id))
        {
            // If the entity is in the Tilemap, remove it before it is freed.
            _remove_entity_from_tilemap(id);
        }

        // Flag this entity as deactivated.
        targ_tracker->is_entity_active = false;

        // Add this ID to the queue of available ids.
        available_ids.push(id);
    }

    /**
     * @brief Returns true if the entity with the passed ID is in the Tilemap.
     * 
     * @param id ID of the entity to check.
     */
    bool is_entity_in_tilemap(const entity_id id) const
    {
        EntityTracker* targ_tracker {};
        
        try
        {
            // Get the tracker assigned to this entity.
            targ_tracker = &all_entities.at(id);
        }

        // If an error ocurred when finding the tracker, handle it.
        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            // If the entity's ID is not a valid one.
            if(!_does_id_exist(id))
            {
                error_message = "Invalid entity ID: " + std::to_string(id);
            }

            // Output the exception to the crash log file and softly crash.
            OUTPUT_CRASH_DETAILS(error_message);
            exit(1);
        }

        // If the x and y coordinate in the tracker are less than the size of the Tilemap's
        // dimensions, the entity is in the Tilemap.
        return targ_tracker->x_position < N && targ_tracker->y_position < N;
    }

    /**
     * @brief Returns true if the the passed ID is being used by an entity.
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    bool is_entity_id_active(const entity_id id) const
    {
        EntityTracker* targ_tracker {};
        
        try
        {
            // Get the tracker assigned to this entity.
            targ_tracker = &all_entities.at(id);
        }

        // If an error ocurred when finding the tracker, handle it.
        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            // If the entity's ID is not a valid one.
            if(!_does_id_exist(id))
            {
                error_message = "Invalid entity ID: " + std::to_string(id);
            }

            // Output the exception to the crash log file and softly crash.
            OUTPUT_CRASH_DETAILS(error_message);
            exit(1);
        }

        return targ_tracker->is_entity_active;
    }

    entity_id register_entity(const T& entity)
    {
        entity_id id;

        // If a recycled ID is available.
        if(available_ids.size() > 0)
        {
            id = available_ids.front();
            available_ids.pop();

            all_entities.at(id).entity = entity;
            all_entities.at(id).is_entity_active = true;
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

    /** Returns a readonly reference to the underlying 2D Array of the Tilemap. */
    const content_type& get_contents() const { return contents; }

    /** Returns a readonly reference to the list of entities at the passed position. */
    const std::list<T>& get_entities_at_position(uint16_t x,
        uint16_t y) const
    {
        try
        {
            return contents.at(y).at(x);
        }

        // If an error ocurred while retrieving the list, handle it.
        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            // The x position is out of bounds.
            if(x >= N)
            {
                error_message = "Invalid x position: " + std::to_string(x);
            }

            // The y position is out of bounds.
            else if(y >= N)
            {
                error_message = "Invalid y position: " + std::to_string(y);
            }

            OUTPUT_CRASH_DETAILS(error_message);
            exit(1);
        }
    }

    /**
     * @brief Returns a readonly reference to the tracker data for the entity with the passed ID.
     * 
     * @attention If the passed ID has been freed and not reassigned, the returned tracker is not 
     * defined for an entity and holds junk data. 
     * 
     * @param id ID of the entity to return the tracker for.
     */
    const EntityTracker& get_tracker_data(const entity_id id) const 
    { 
       try
        {
            return all_entities.at(id);
        }

        // If an error ocurred while retrieving the tracker, handle it.
        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            // If the entity's ID is not a valid one.
            if(!_does_id_exist(id))
            {
                error_message = "Invalid entity ID: " + std::to_string(id);
            }

            // Output the exception to the crash log file and softly crash.
            OUTPUT_CRASH_DETAILS(error_message);
            exit(1);
        }
    }

private:

    // Members

    /** Underlying contents of the Tilemap that stores all added entities. */
    content_type contents;

    // Ids that have been decommissioned and can be recycled.
    std::queue<entity_id> available_ids;
 
    /** Vector of all tracker objects for entities. The index of the vector represents the id of 
     * the object that is registered. */
    std::vector<EntityTracker> all_entities; 


    // Methods

    /**
     * @brief Underlying method for removing the entity_id from the tilemap. Skips updating the 
     * targ tracker's position, as these are handled case-by-case in the public removal methods.
     * 
     * @param id ID of the entity to remove.
     */
    void _remove_entity_from_tilemap(const entity_id id, uint16_t x_position, uint16_t y_position)
    {
        EntityTracker* targ_tracker {};

        // Iterator to the beginning of the target list at the entity's position.
        typename std::list<T>::const_iterator it_begin {};
        
        // Iterator to the end of the target list at the entity's position.
        typename std::list<T>::const_iterator it_end {};

        try 
        {
            // Get the tracker assigned to this ID.
            targ_tracker = &all_entities.at(id);

            it_begin = contents.at(y_position).at(x_position).begin();
            it_end = contents.at(y_position).at(x_position).end();
        }

        // If an error ocurred when removing the entity, handle it.
        catch(const std::out_of_range& e)
        {
            std::string error_message = e.what();

            // If the entity's ID is not a valid one.
            if(!_does_id_exist(id))
            {
                error_message = "Invalid entity ID: " + std::to_string(id);
            }

            // The x position is out of bounds.
            if(x_position >= N)
            {
                error_message = "Invalid x position: " + std::to_string(x_position);
            }

            // The y position is out of bounds.
            else if(y_position >= N)
            {
                error_message = "Invalid y position: " + std::to_string(y_position);
            }

            // Output the exception to the crash log file and softly crash.
            OUTPUT_CRASH_DETAILS(error_message);
            exit(1);
        }

        // Iterate over each entity_id in the list at the position.
        while(it_begin != it_end)
        {
            // If the parsed entity is the entity to be removed.
            if(*it_begin == targ_tracker->entity)
            {
                // Erase the entity at the iterator's position.
                contents.at(y_position).at(x_position).erase(it_begin);
                return;
            }

            ++it_begin;
        }

        // If this part of the function is reached, the entity was not found. Output an error and 
        // softly crash the program.

        OUTPUT_CRASH_DETAILS("Entity was requested to be removed, but could not be found.");
    }

    // Returns true if the passed ID exists.
    bool _does_id_exist(const entity_id id) const { return id < all_entities.size(); }

    /** Returns true if the passed ID is present in the tilemap. Since this is an internal method,
     * it is assumed that the ID is valid. */
    bool _is_id_in_tilemap(const entity_id id) const { return all_entities.at(id).x_position < N; }

    /**
     * Returns true if the passed ID is registered to an active entity, and not an ID that has been
     * freed. */
    bool _is_entity_active(const entity_id id) const { return all_entities.at(id).entity; }
};
