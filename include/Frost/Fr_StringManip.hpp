/**
 * @file Fr_StringManip.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Collection of string manipulation methods.
 * @version 0.1
 * @date 24-08-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>


namespace Frost
{

/** Uses the InputHandler to observe user inputs, and makes modifications to the passed string
 * based on those inputs.
 * 
 * @param str String to modify. Passed by reference.
 */
void handle_input_for_string_manipulation(std::string& str);

/** Places new lines in a string after each character that breaches the line limit. This will 
 * turn a long string into a modified one that will have newlines placed such that when each line  
 * is printed to a console or file it will not exceed the set line limit. */
void configure_string_with_line_limit(std::string& str, uint8_t line_limit);


/** Trims a string like you would expect text to be trimmed inside notepad when you press
 * Ctrl + Backspace. If the last character of the string is not a space, iterate backwards until
 * the first space is found, and substring the str until that point. If the last character of hte 
 * string is a space, iterate backwards until the first non space character is found, then perform 
 * the first process of finding the first space character and substringing until that point.
 */
void trim_string_with_ctrl_backspace_behavior(std::string& str);

/** Removes all zeros starting from the beginning of the string, up until the first non zero 
 * character is found.
 * 
 * @param str String to modify. Passed by reference.
 */
void remove_first_zeros(std::string& str);

/** Attempts to convert the passed string to a uint8_t. Returns a max uint8_t if it fails. */
uint8_t str_to_uint8(std::string& str);

/** Attempts to convert the passed string to a uint16_t. Returns a max uint8_t if it fails. */
uint16_t str_to_uint16(std::string& str);

};
