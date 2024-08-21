/**
 * @file EventSystem.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for components for registering and invoking stored "events" or function 
 *        pointers. 
 * @version 0.1
 * @date 2024-08-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

 #include <functional>
 #include <unordered_map>
 #include <string>

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif


/** @brief Static class that handles storing and invoking events (Function pointers.) 
 * 
 * Use the $subscribe() method to add register a function pointer to an event token. When the 
 * $invoke_event() method is called with an event token, all subscribed function pointers that are
 * registered to that event token will be called. */
class EventSystem
{

public:

    /** Subscribes the $callback function to the $callback_token. */
    template <typename T>
    static void subscribe(std::string callback_token, T* obj, 
        std::function<void(T&)> callback) 
    {
        s_subscriptions[callback_token].push_back(new Subscription<T>(obj, callback));
    }

    static void invoke_event(std::string callback_token) 
    {
        // Find the vector of callbacks this token is associated with.
        auto it = s_subscriptions.find(callback_token);

        // Token is invalid
        if(it == s_subscriptions.end())
        {
            #ifdef FROST_DEBUG

            ProgramOutputHandler::log("EventSystem::trigger_callback(): Callback tokennot found ->"
                + it->first, Frost::WARN);
            #endif
            return;
        }

        // Token is valid.

        // Iterate through each function pointer associated with this token.
        for(auto sub : it->second)
        {
            // Invoke this callback.
            sub->invoke();
        }
    }


private:

    // Classes 

    /** Provides base class for Subscriptions to inherit. */
    class SubscriptionBase {

    public:
        
        virtual ~SubscriptionBase() = default;
        virtual void invoke() = 0;

    };

    /** Stores a function pointer along with the object that holds it to allow it to be invoked by
     * by calling the $invoke() method. */
    template <typename T>
    class Subscription : public SubscriptionBase {

    public:
        
        Subscription(T* obj, std::function<void(T&)> callback)
            : m_OBJECT_PTR(obj), m_CALLBACK_METHOD(callback) {}

        void invoke() final 
        {
            m_CALLBACK_METHOD(*m_OBJECT_PTR);
        }

    private:

        T* m_OBJECT_PTR; // The object that contains the callback method.
        std::function<void(T&)> m_CALLBACK_METHOD; // Method that will be called.
        
    };


    // Members 

    // Event identifiers to their vector of Subscriptions to invoke.
    static std::unordered_map<std::string, std::vector<SubscriptionBase*>> s_subscriptions;

};