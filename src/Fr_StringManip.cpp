#include <stdexcept>

#include "Fr_StringManip.hpp"
#include "Fr_IntLimits.hpp"

void Frost::configure_string_with_line_limit(std::string& content, uint8_t line_limit)
{
    // If the content does not need to be trimmed
    if(line_limit >= content.size()) return;

    // Iterator that parses over the string. Starts at the line limit, since it is known that 
    // the size of the content needs to be trimmed, and a newline should be placed at this index
    uint64_t index_iterator = line_limit;

    // Once the index_iterator breaches the end of the content, it's known that a newline has been
    // placed after each portion equal to the line limit.
    while(index_iterator < content.size())
    {
        content.insert(content.begin() + index_iterator, '\n');

        // Increment the index by the line_limit amount, as this will be the next supposed place to
        // put a new line. 
        index_iterator += line_limit;
    }
}

uint8_t Frost::str_to_uint8(std::string& _str)
{
    // The number this string represents is negative
    if(_str.at(0) == '-') return 0;

    try
    {
        return std::stoi(_str);
    } 
    
    catch(const std::out_of_range& e)
    {
        return Frost::UINT8_LIMIT;
    }
}

uint16_t Frost::str_to_uint16(std::string& _str)
{
    // The number this string represents is negative
    if(_str.at(0) == '-') return 0;

    try
    {
        return std::stoi(_str);
    } 
    
    catch(const std::out_of_range& e)
    {
        return Frost::UINT16_LIMIT;
    }
}
