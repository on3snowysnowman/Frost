#include <SDL2/SDL.h>

#include "InputHandler.h"


// Static Members

std::vector<int32_t> InputHandler::s_raw_pressed_keys;

std::unordered_set<int32_t> InputHandler::s_pressed_keys;

std::unordered_map<int32_t, uint64_t> InputHandler::s_delayed_keys;


// Public

void InputHandler::flag_key_pressed(int32_t key)
{
    // Add this key to the raw keys pressed this frame.
    s_raw_pressed_keys.push_back(key);

    // Add this to the currently pressed keys.
    s_pressed_keys.emplace(key);
}

void InputHandler::flag_key_released(int32_t key)
{
    // Erase this key from the currently pressed keys.
    s_pressed_keys.erase(key);
}

void InputHandler::clear_raw_keys() { s_raw_pressed_keys.clear(); }

bool InputHandler::is_key_pressed(int32_t key) 
{ return s_pressed_keys.find(key) != s_pressed_keys.end(); }

bool InputHandler::is_key_pressed_and_available(int32_t key)
{
    // The key is not pressed.
    if(!is_key_pressed(key)) return false;

    // The key is not delayed.
    if(s_delayed_keys.find(key) == s_delayed_keys.end());

    // The key is pressed and delayed, check if the current timestamp is greater than the delay
    // timestamp.

    // If the current timestamp is less than the delayed timestamp.
    if(s_delayed_keys.at(key) > SDL_GetTicks64()) return false;

    // The current timestamp has passed the delayed timestamp, therefore this key is available.
    
    // Remove the key from the delayed keys, since it is no longer delayed.
    s_delayed_keys.erase(key);
    
    return true;
}

const std::vector<int32_t>& InputHandler::get_raw_pressed_keys() 
{ return s_raw_pressed_keys; }

const std::unordered_set<int32_t> InputHandler::get_pressed_keys()
{ return s_pressed_keys; }
