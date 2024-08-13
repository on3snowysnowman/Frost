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

/** Starting from the end of a string, iterate backwards and stop at the first instance of the 
 * passed 'c' that exists. The string will be portioned up until that point. If the 
 * include_searched_char is set to true, the index of 'c' will be included in the portioning.
 * If the searched for char wasn't found, the string is untouched.
 * 
 * @param str String to modify. Passed by reference.
 * @param c Char to search for.
 * @param include_searched_char Whether to include the searched char in the portion.
 */
void trim_string_to_first_character_from_end(std::string& str, char c, 
    bool include_searched_char = false);

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
