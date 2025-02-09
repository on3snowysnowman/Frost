#pragma once

#include <cstdint>
#include <vector>
#include <unordered_set>
#include <unordered_map>

// Ascii value for a pressed key on the keyboard. 
typedef int32_t Key;


/** 
 * @brief Handles tracking pressed keys on the keyboard, along with "blocking" 
 * said keys so they cannot be registered as pressed again until a delay has 
 * been reached.
 * 
 * This class includes functionality for tracking which key is currently 
 * pressed, getting the "raw" key data (Think of when you're typing and you 
 * hold a key down, it spits out one character, then spams a bunch of them 
 * after a short delay), along with assigning a delay to a key, so that it 
 * can't be "used" again until it has passed the delay timestamp.
 * 
 * This is a static only member class, and therefore contains single instances 
 * of components. Maintain caution when using with multiple threads. 
 */
class InputHandler
{

public:

    /** 
     * @brief Delay a key from being active for a set amount of miliseconds.
     * 
     * @param key Key to delay.
     * @param miliseconds Number of miliseconds.
     * 
     */
    static void delay_key(Key key, uint16_t miliseconds = 160);

    /** 
     * @brief Blocks a key from being available until the physical keyboard key 
     * is released. 
     * 
     * @param key Key to 
     * 
     * */
    static void block_key_until_released(Key key);

    /** 
     * @brief Signals the InputHandler that the passed key has been physically 
     * pressed.
     * 
     * @attention This function is called by the Engine when handling keyboard
     * events, the user should not call this function.
     * 
     * @param key Pressed key.
     * 
     */
    static void _flag_key_pressed(Key key);
    
    /** 
     * @brief Signals the InputHandler that the passed key has been physically 
     * released.
     * 
     * @attention This function is called by the Engine when handling keyboard
     * events, the user should not call this function.
     * 
     * @param key Released Key.
     * 
     */
    static void _flag_key_released(Key key);

    /** 
     * @brief Clears the tracked raw keys and flags the pressed available keys
     * to be regenerated on their next fetch request.
     * 
     * Called once per frame.
     * 
     * @attention This function is called by the Engine each frame, the user 
     * should not call this function.
     * 
     * */
    static void _reset_tracked_keys();

    /** 
     * @brief Returns true if the passed key is physically pressed down, 
     * regardless if it is delayed or not.
     * 
     * @param key Key to check.
     * 
     */
    static bool is_key_pressed(Key key);
    
    /** 
     * Returns true if the passed key is physically pressed down, and is not 
     * blocked by a delay.
     * 
     * @param key Key to check.
     * 
    */
    static bool is_key_pressed_and_available(Key key);

    /** 
     * @brief Returns a vector of the pressed keys that are available and not 
     * delayed. 
     * 
     * */
    static const std::vector<Key>& get_pressed_and_available_keys();

    /** 
     * @brief Returns a const reference to the internal raw pressed keys. 
     * 
     * */
    static const std::vector<Key>& get_raw_pressed_keys();

    /** 
     * @brief Returns a const reference to the internal pressed keys. 
     * 
     * */
    static const std::unordered_set<Key> get_pressed_keys();

private:

    // Members

    /** 
     * If the pressed available keys vector has already been generated this 
     * frame. */
    static bool s_is_available_keys_generated;


    // Methods

    /** Returns true if the passed key that is pressed is available. 
     * 
     * @param key Key to check.
     * 
    */
    static bool _is_pressed_key_available(Key key);

    /** 
     * @brief Returns true if the passed and delayed `key` is available to use 
     * after checking and processing its delay.
     * 
     * Processes a key that is pressed and delayed, checking if the current 
     * timestamp has surpassed the target delay timestamp. If it has exceeded 
     * the timestamp, remove the key from the delayed keys vector. Returns true 
     * if the current timestamp has surpassed the delayed timestamp, meaning 
     * the key is available to use.
     * 
     * This is an internal function, so it is guaranteed that the `key` is 
     * pressed and delayed.
     * 
     */
    static bool _check_and_handle_key_delay(Key key);

    /** Keys that are pressed and are not currently delayed. This vecotr is filled when the 
     * get_pressed_and_available_keys() method is called, and its contents will persist until the
     * next frame when the clear_key_vectors() method is called. */
    static std::vector<Key> s_pressed_available_keys;

    // Raw pressed key from a keyboard, as if you were typing in a text document
    static std::vector<Key> s_raw_pressed_keys; 

    // Keys that are currently pressed.
    static std::unordered_set<Key> s_pressed_keys;

    // Keys that have been assigned a delay.
    static std::unordered_map<Key, uint32_t> s_delayed_keys;

};