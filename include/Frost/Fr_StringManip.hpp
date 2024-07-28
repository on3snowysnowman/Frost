#pragma once

#include <string>
#include <cstdint>

namespace Frost
{

/** Places new lines in a string after each character that breaches the line limit. This will 
 * turn a long string into a modified one that will have newlines placed such that when each line  
 * is printed to a console or file it will not exceed the set line limit. */
void configure_string_with_line_limit(std::string& content, uint8_t line_limit);

/** Attempts to convert the passed string to a uint8_t. Returns a max uint8_t if it fails. */
uint8_t str_to_uint8(std::string& _str);

/** Attempts to convert the passed string to a uint16_t. Returns a max uint8_t if it fails. */
uint16_t str_to_uint16(std::string& _str);

};
