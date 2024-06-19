#pragma once

#include <string>
#include <cstdint>

namespace Frost
{

/** Places new lines in a string after each character that breaches the line limit. This will 
 * turn a long string into a modified one that will have newlines placed such that when each line  
 * is printed to a console or file it will not exceed the set line limit. */
void configure_string_with_line_limit(std::string& content, uint8_t line_limit);

};
