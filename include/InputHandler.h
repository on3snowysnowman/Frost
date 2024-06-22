#pragma once

#include <cstdint>
#include <vector>
#include <unordered_set>
#include <unordered_map>



/** Static class that handles pressed keys on the keyboard. This class includes functionality for
 * tracking which key is currently pressed, getting the "raw" key data (Think of when you're typing
 * and you hold a key down, it spits out one character, then spams a bunch of them after a short 
 * delay), along with assigning a delay to a key, so that it can't be "used" again until it has 
 * passed the delay timestamp.
 */
class InputHandler
{

public:

    /** Flags the InputHandler that the passed key is pressed.
     * 
     * @param key Pressed key.
     */
    static void flag_key_pressed(int32_t key);
    
    /** Flags the InputHandler that the passed key is released.
     * 
     * @param key Released Key.
     */
    static void flag_key_released(int32_t key);

    /** Called once per frame. Clears the raw key data, so that the new raw keys can be registered
     * this frame. */
    static void clear_raw_keys();

    /** Returns true if the passed key is pressed down, regardless if it is delayed or not.
     * 
     * @param key Key to check.
     */
    static bool is_key_pressed(int32_t key);
    
    /** Returns true if the passed key is pressed down, and is not delayed by the InputHandler. 
     * 
     * @param key Key to check.
    */
    static bool is_key_pressed_and_available(int32_t key);

    /** Returns a const reference to the internal raw pressed keys. */
    static const std::vector<int32_t>& get_raw_pressed_keys();

    /** Returns a const reference to the internal pressed keys. */
    static const std::unordered_set<int32_t> get_pressed_keys();

private:

    // Raw pressed key from a keyboard, as if you were typing in a text document
    static std::vector<int32_t> s_raw_pressed_keys; 

    // Keys that are currently pressed.
    static std::unordered_set<int32_t> s_pressed_keys;

    // Keys that have been assigned a delay.
    static std::unordered_map<int32_t, uint64_t> s_delayed_keys;

};