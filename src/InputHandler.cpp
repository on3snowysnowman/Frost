#include <SDL2/SDL.h>
#include <SDL_stdinc.h>

#include "InputHandler.hpp"
#include "Fr_IntLimits.hpp"

// Static Members

std::vector<Key> InputHandler::s_raw_pressed_keys;

std::unordered_set<Key> InputHandler::s_pressed_keys;

std::unordered_map<Key, uint32_t> InputHandler::s_delayed_keys;


// Public

void InputHandler::delay_key(Key key, uint16_t miliseconds)
{
    s_delayed_keys[key] = SDL_GetTicks64() + miliseconds;
}


void InputHandler::block_key_until_released(Key key)
{
    // Set the delay timestamp equal to the limit of a uint32_t, which realistically, is never 
    // going to be reached.
    s_delayed_keys[key] = Frost::UINT32_LIMIT;
}

void InputHandler::flag_key_pressed(Key key)
{
    // Add this key to the raw keys pressed this frame.
    s_raw_pressed_keys.push_back(key);

    // Add this to the currently pressed keys.
    s_pressed_keys.emplace(key);
}

void InputHandler::flag_key_released(Key key)
{
    // Erase this key from the currently pressed keys.
    s_pressed_keys.erase(key);

    // Erase this key from the delayed keys, if it is not delayed this will be handled efficiently
    // by std::unordered_map
    s_delayed_keys.erase(key);
}

void InputHandler::clear_raw_keys() { s_raw_pressed_keys.clear(); }

bool InputHandler::is_key_pressed(Key key) 
{ return s_pressed_keys.find(key) != s_pressed_keys.end(); }

bool InputHandler::is_key_pressed_and_available(Key key)
{
    return is_key_pressed(key) && _is_pressed_key_available(key);
}

std::vector<Key> InputHandler::get_pressed_and_available_keys()
{
    std::vector<Key> pressed_available_keys;

    for(const Key& key : s_pressed_keys)
    {
        if(_is_pressed_key_available(key)) pressed_available_keys.push_back(key);
    }

    return pressed_available_keys;
}

const std::vector<Key>& InputHandler::get_raw_pressed_keys() 
{ return s_raw_pressed_keys; }

const std::unordered_set<Key> InputHandler::get_pressed_keys()
{ return s_pressed_keys; }


// Private

bool InputHandler::_is_pressed_key_available(Key key)
{
    // If this key is not delayed.
    if(s_delayed_keys.find(key) == s_delayed_keys.end()) return true;

    // If this key is delayed, check and handle the delay.
    return _check_and_handle_key_delay(key);
}

bool InputHandler::_check_and_handle_key_delay(Key key)
{
    // Since this is an internal method, it can be assumed that the key is certain to be delayed if
    // this method is called.

    // If the delayed timestamp is greater than the current timestamp.
    if(s_delayed_keys.at(key) > SDL_GetTicks64())
    {
        return false;
    }

    // Remove the key from the delayed keys, since it is no longer delayed.
    s_delayed_keys.erase(key);

    return true;
}
