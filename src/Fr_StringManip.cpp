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

#include <iostream>
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

    int index = 0;

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

                // Control is not pressed.
                
                // This snippet of code finds the beginning of the first word from the end
                // of the string and trims it to that portion.

                // If the string is not long enough to worry about this functionality.
                if(str.size() == 1) continue;

                index = str.size() - 2;

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
                    continue;
                }

                // Iterate backwards until the first space is found.

                while(index > 0)
                {
                    if(str.at(index) == ' ') break;

                    --index;
                }

                str = str.substr(0, index);
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

