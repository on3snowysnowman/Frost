/**
 * @file EventSystem.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation for components for registering and invoking stored "events" or function 
 *        pointers. 
 * @version 0.1
 * @date 2024-08-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */



 #include "EventSystem.hpp"


// Static Members

std::unordered_map<std::string, std::vector<EventSystem::SubscriptionBase*>> 
    EventSystem::s_subscriptions;
