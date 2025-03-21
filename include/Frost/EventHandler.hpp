/**
 * @file EventHandler.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class declaration and implementation.
 * @version 0.2
 * @date 2024-08-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>

#include "TextFileHandler.hpp"
#include "CrashOutputHandler.hpp"

using event_id = uint64_t;


/** 
 * @brief Static class that allows any function, with any arguments and return type, to be 
 * subscribed to an 'invoke_key' and to call said function at any time and anywhere using said key.
 * 
 * When an event is subscribed, an event_id token will be returned. This token is the identifier 
 * of the unique event that was created for the subscribed function. This token is only used when
 * unsubscribing an event. If it is known that an event will be subscribed for the lifetime of the 
 * program, the event_id token can be disregarded.
 * 
 * Multiple events can be su
 * Invoke an event by passing the invoke key to the 'invoke_event' method. Pass the proper template
 * arguments of the subscribed event in with the invoke_key. 
 */
class EventHandler
{

public:

    /**
     * @brief "Unsubscribes" an event from an invoke key, removing it from 
     * being associated with it and cleaning internal memory.
     * 
     * @param invoke_key Invoke key to remove event from.
     * @param id ID of the subscribed event.
     * 
     */
    static void unsubscribe(const std::string& invoke_key, event_id id)
    {
        // If this invoke key doesn't exist.
        if(s_invoke_keys_to_events.find(invoke_key) == s_invoke_keys_to_events.end())
        {
            OUTPUT_CRASH_DETAILS(" where 'invoke_key' = '" + invoke_key + "' -> No such invoke key exists.\n");
            exit(1);
        }

        std::vector<EventBase*>::const_iterator it = 
            s_invoke_keys_to_events.at(invoke_key).begin();

        // Iterate through each event in the list subscribed to the passed invoke key.
        while(it != s_invoke_keys_to_events.at(invoke_key).end())
        {
            // These aren't the events you're looking for.
            if((*it)->get_id() != id)
            {
                ++it;
                continue;
            }

            _handle_remove_event(invoke_key, it);
            return;
        }

        // If the function makes it to this point, the ID wasn't found in the vector.

        OUTPUT_CRASH_DETAILS(" where 'invoke_key' = '" + invoke_key + "' & 'id' = " + 
            std::to_string(id) + "' -> specified id was not found in the list of subscribed events"
            " for this invoke key.\n");
        exit(1);
    }

    /** 
     * 
     * @brief Register an event with the EventHandler, passing 'invoke_key' as the token used 
     * to invoke the passed method. 
     * 
     * @param invoke_key Unique token to register this event under. 
     * @param target_function Function to call when the event is invoked. 
     * 
     * */
    template<typename ReturnType, typename... FunctionArgs>
    static const event_id& register_event(const std::string& invoke_key, 
        std::function<ReturnType(FunctionArgs...)> target_function)
    {
        Event<ReturnType, FunctionArgs...>* event = 
            new Event<ReturnType, FunctionArgs...>(std::move(target_function));

        s_invoke_keys_to_events[invoke_key].push_back(event);

        return event->get_id();
    }

    /**
     * @brief Retrieves the list of events that are tied to the passed 'invoke_key' token, and 
     * invokes each of them.
     * 
     * If there is more than one function tied to the 'invoke_key', only the last subscribed 
     * function's return value will be returned. 
     * 
     * @param invoke_key Unique token to lookup the events to invoke.
     * @param args Arguments to pass to each invoked event.
     * 
     */
    template<typename ReturnType, typename... FunctionArgs>
    static ReturnType invoke_event(const std::string& invoke_key, FunctionArgs... args)
    {
        // Attempt to get an iterator to the position in the map where the specified invoke key is.
        std::unordered_map<std::string, std::vector<EventBase*>>::const_iterator it 
            = s_invoke_keys_to_events.find(invoke_key);

        // If this invoke key does not exist
        if(it == s_invoke_keys_to_events.end())
        {
            OUTPUT_CRASH_DETAILS(" where 'invoke_key' = '" + invoke_key + "' -> No such invoke key exists.\n");
            exit(1);
        }

        // It is guaranteed that at least one event will exist in the vector, as vectors are 
        // removed from the unordered_map as they are emptied through the 'unsubscribe' method.
    
        for(uint32_t i = 0; i < it->second.size() - 1; ++i)
        {
            Event<ReturnType, FunctionArgs...>* event = 
                static_cast<Event<ReturnType, FunctionArgs...>*>(it->second.at(i)); 

            event->invoke(std::forward<FunctionArgs>(args)...);
        }

        return static_cast<Event<ReturnType, FunctionArgs...>*>(
            it->second.back())->invoke(std::forward<FunctionArgs>(args)...);
    }

private:

    // Classes / Structs

    /** 
     * @brief Base Event class, only exists to allow derived child class types to be placed in vector
     * memory as pointers to this base class. 
     * 
     * */
    class EventBase
    {   
    
    public:

        EventBase() : m_ID(_get_available_id()) {}

        const event_id& get_id() const { return m_ID; }

    private:

        const event_id m_ID;

    };

    /** 
     * @brief Derived Event class for specific function signatures. 
     * 
     * */
    template <typename ReturnType, typename... FunctionArgs>
    class Event : public EventBase
    {

    public:

        Event(std::function<ReturnType(FunctionArgs...)>&& target_function)
            { m_target_function = target_function; }

        ReturnType invoke(FunctionArgs... args) 
            { return m_target_function(std::forward<FunctionArgs>(args)...); }

    private:

        std::function<ReturnType(FunctionArgs...)> m_target_function;

    };


    // Members 

    // The next available ID that can be assigned.
    static inline event_id s_next_id {};

    // Queue of IDs that have been decomissioned and can be reused.
    static inline std::queue<event_id> s_available_ids;

    // Invoke keys to their respective list of events. 
    static inline std::unordered_map<std::string, std::vector<EventBase*>> s_invoke_keys_to_events;


    // Methods

    /** 
     * @brief Generates an ID for a newly created Event. This ID can either be 
     * a newly generated one, or an ID that has existed before and is recycled. 
     * 
     * */
    static event_id _get_available_id()
    {
        event_id id;

        // If there are availabled ids to be used.
        if(s_available_ids.size() > 0)
        {
            id = s_available_ids.front();
            s_available_ids.pop();
            return id;
        }

        // Get a new ID that hasn't existed yet.
        id = s_next_id;
        ++s_next_id;
        
        return id;
    }

    /** 
     * @brief Handles removing and deleting the event at the iterator 'it', adding its ID to the 
     * list of avaiable decomissioned Id's.
     * 
     * Additionally, checks the bounds of the vector that had the event in it. If it is 0, it is 
     * deleted from the invoke key map. 
     * 
     * */
    static void _handle_remove_event(const std::string& invoke_key, std::vector<EventBase*>::const_iterator it)
    {
        // Delete the event from heap memory.
        delete *it;

        // Erase the dangling pointer from the vector.
        s_invoke_keys_to_events.at(invoke_key).erase(it);

        // If the vector is empty, remove it from the map.
        if(s_invoke_keys_to_events.at(invoke_key).size() == 0) 
            s_invoke_keys_to_events.erase(invoke_key);
    }
};
