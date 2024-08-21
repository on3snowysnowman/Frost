/**
 * @file Fr_StringManip.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Implementation of string manipulation methods.
 * @version 0.1
 * @date 24-08-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <SDL2/SDL_keycode.h>
#include <stdexcept>

#include "InputHandler.hpp"
#include "Fr_StringManip.hpp"
#include "Fr_IntLimits.hpp"

void Frost::handle_input_for_string_manipulation(std::string& str)
{   
    // Ascii characters to their respective characters when pressed with shift.
    static const std::unordered_map<char, char> KEYS_TO_SHIFTED_KEYS
    {
        {'\'', '"'},
        {',', '<'},
        {'-', '_'},
        {'.', '>'},
        {'/', '?'},
        {';', ':'},
        {'=', '+'},
        {'[', '{'},
        {'\\', '|'},
        {']', '}'},
        {'0', ')'},
        {'1', '!'},
        {'2', '@'},
        {'3', '#'},
        {'4', '$'},
        {'5', '%'},
        {'6', '^'},
        {'7', '&'},
        {'8', '*'},
        {'9', '('},
    };

    if(InputHandler::is_key_pressed(SDLK_LSHIFT))
    {
        for(int32_t key : InputHandler::get_raw_pressed_keys())
        {
            switch(key)
            {
                case SDLK_BACKSPACE:

                    if(str.size() == 0) continue;

                    if(InputHandler::is_key_pressed(SDLK_LCTRL))
                    {
                        str.clear();
                        continue;
                    }

                    str.pop_back();
                    continue;
                
                case SDLK_SPACE:

                    str.push_back(' ');
                    continue;

                case SDLK_TAB:

                    str.append("    ");
                    continue;
            }

            if(key >= 97 && key <= 122)
            {
                str.push_back(char(key - 32));
                continue;
            }

            else if(KEYS_TO_SHIFTED_KEYS.find(key) != KEYS_TO_SHIFTED_KEYS.end())
            {
                str.push_back(KEYS_TO_SHIFTED_KEYS.at(key));
                continue;
            }
        }

        return;
    }

    // Shift is not pressed.

    for(int32_t key : InputHandler::get_raw_pressed_keys())
    {
        switch(key)
        {
            case SDLK_BACKSPACE:    

                if(str.size() == 0) continue;

                // If control is not pressed.
                if(!InputHandler::is_key_pressed(SDLK_LCTRL))
                {
                    str.pop_back();
                    continue;
                }

                // Control is pressed.

                trim_string_with_ctrl_backspace_behavior(str);
                continue;
                

            case SDLK_SPACE:

                str.push_back(' ');
                continue;
            
            case SDLK_TAB:  

                str.append("    ");
                continue;
        }

        if(key >= 33 && key <= 126) 
        {
            str.push_back(key);
        }
    }
}

void Frost::configure_string_with_line_limit(std::string& str, uint8_t line_limit)
{
    // If the str does not need to be trimmed
    if(line_limit >= str.size()) return;

    // Iterator that parses over the string. Starts at the line limit, since it is known that 
    // the size of the str needs to be trimmed, and a newline should be placed at this index
    uint64_t index_iterator = line_limit;

    // Once the index_iterator breaches the end of the str, it's known that a newline has been
    // placed after each portion equal to the line limit.
    while(index_iterator < str.size())
    {
        str.insert(str.begin() + index_iterator, '\n');

        // Increment the index by the line_limit amount, as this will be the next supposed place to
        // put a new line. 
        index_iterator += line_limit;
    }
}

void Frost::trim_string_with_ctrl_backspace_behavior(std::string& str)
{
    // If the string is not long enough to worry about this functionality.
    if(str.size() <= 1) return;

    int index = str.size() - 2;

    // If the character at the end of the string is a space
    if(*(--str.end()) == ' ')
    {
        // Iterate backwards until the first non space character is found.

        while(index > 0)
        {
            if(str.at(index) != ' ') break;

            --index;
        }

        // Use ++index, as the character that is currently parsed is the first 
        // character before the spaces that were skipped, and that should not be 
        // trimmed out. So, increment index inside substr to include the character that
        // is currently parsed.
        str = str.substr(0, ++index);
        return;
    }

    // Iterate backwards until the first space is found.

    while(index > 0)
    {
        if(str.at(index) == ' ') break;

        --index;
    }

    str = str.substr(0, index);
}

void Frost::remove_first_zeros(std::string& str)
{
    // There are no trailing zeros since there is not more than 1 character.
    if(str.size() <= 1) return;

    int index = 0;

    while(index < str.size())
    {
        if(str.at(index) != '0') break;

        ++index;
    }

    str = str.substr(index);
}

uint8_t Frost::str_to_uint8(std::string& str)
{
    // The number this string represents is negative
    if(str.at(0) == '-') return 0;

    try
    {
        return std::stoi(str);
    } 
    
    catch(const std::out_of_range& e)
    {
        return Frost::UINT8_LIMIT;
    }
}

uint16_t Frost::str_to_uint16(std::string& str)
{
    // The number this string represents is negative
    if(str.at(0) == '-') return 0;

    try
    {
        return std::stoi(str);
    } 
    
    catch(const std::out_of_range& e)
    {
        return Frost::UINT16_LIMIT;
    }
}

