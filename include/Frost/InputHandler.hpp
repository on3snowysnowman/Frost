#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>

typedef int32_t Key;


/** Static class that handles pressed keys on the keyboard. This class includes functionality for
 * tracking which key is currently pressed, getting the "raw" key data (Think of when you're typing
 * and you hold a key down, it spits out one character, then spams a bunch of them after a short 
 * delay), along with assigning a delay to a key, so that it can't be "used" again until it has 
 * passed the delay timestamp.
 */
class InputHandler
{

public:

    /** Delay a key from being active for a set amount of miliseconds.
     * 
     * @param key Key to delay.
     * @param miliseconds Number of miliseconds.
     */
    static void delay_key(Key key, uint16_t miliseconds = 200);

    /** Blocks a key from being available until it is released and pressed again. */
    static void block_key_until_released(Key key);

    /** Flags the InputHandler that the passed key is pressed.
     * 
     * @param key Pressed key.
     */
    static void flag_key_pressed(Key key);
    
    /** Flags the InputHandler that the passed key is released.
     * 
     * @param key Released Key.
     */
    static void flag_key_released(Key key);

    /** Called once per frame. Clears the raw key data, so that the new raw keys can be registered
     * this frame. */
    static void clear_raw_keys();

    /** Returns true if the passed key is pressed down, regardless if it is delayed or not.
     * 
     * @param key Key to check.
     */
    static bool is_key_pressed(Key key);
    
    /** Returns true if the passed key is pressed down, and is not delayed by the InputHandler. 
     * 
     * @param key Key to check.
    */
    static bool is_key_pressed_and_available(Key key);

    /** Returns a vector of the pressed keys that are available and not delayed. */
    static std::vector<Key> get_pressed_and_available_keys();

    /** Returns a const reference to the internal raw pressed keys. */
    static const std::vector<Key>& get_raw_pressed_keys();

    /** Returns a const reference to the internal pressed keys. */
    static const std::unordered_set<Key> get_pressed_keys();

private:

    /** Returns true if the passed key that is pressed is available. 
     * 
     * @param key Key to check.
    */
    static bool _is_pressed_key_available(Key key);

    /** Processes a key that is pressed and delayed, checking if the current timestamp has
     * surpassed the target delay timestamp. If it has exceeded the timestamp, remove the key from
     * the delayed keys vector. Returns true if the current timestamp has surpassed the delayed 
     * timestamp, meaning the key is available to use.
     */
    static bool _check_and_handle_key_delay(Key key);

    // Raw pressed key from a keyboard, as if you were typing in a text document
    static std::vector<Key> s_raw_pressed_keys; 

    // Keys that are currently pressed.
    static std::unordered_set<Key> s_pressed_keys;

    // Keys that have been assigned a delay.
    static std::unordered_map<Key, uint64_t> s_delayed_keys;

};