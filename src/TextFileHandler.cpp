/**
 * @file TextFileHandler.cp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class implementation.
 * @version 0.1
 * @date ?
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <sstream>

#include "TextFileHandler.hpp"
#include "FileSystemHandler.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif


// Public

void TextFileHandler::add_to_buffer(std::string str) { _get_buffer().append(str); }

void TextFileHandler::clear_buffer() { _get_buffer().clear(); }

bool TextFileHandler::clear_file(std::string file_path)
{
    std::ofstream& s_output_stream = _get_output_stream();

    // If the path does not exist.
    if(!FileSystemHandler::does_directory_exist(file_path))
    {
        return FileSystemHandler::make_file(file_path);

        // #ifdef FROST_DEBUG

        // ProgramOutputHandler::log("TextFileHandler.clear_file() -> Directory doesn't exist: \""
        //     + file_path + "\"", Frost::WARN);
        // #endif

        // return false;
    }

    s_output_stream.open(file_path, std::ios::out);

    // If the file failed to open.
    if(!s_output_stream.is_open())
    {
        #ifdef FROST_DEBUG
        
        ProgramOutputHandler::log("TextFileHandler.clear_file() -> Failed to open file: \""
            + file_path + "\"", Frost::WARN);
        #endif

        return false;
    }

    s_output_stream.close();

    return true;
}

bool TextFileHandler::write(std::string file_path, Frost::WriteType write_type, bool clear_buffer)
{
    std::ofstream& s_output_stream = _get_output_stream();
    std::string& s_buffer = _get_buffer();

    // Handle write behavior.
    switch(write_type)
    {
        case Frost::APPEND:

            s_output_stream.open(file_path, std::ios::app);
            break;

        case Frost::OVERWRITE:

            s_output_stream.open(file_path, std::ios::out);
            break;
    }

    // If the file failed to open.
    if(!s_output_stream.is_open())
    {
        #ifdef FROST_DEBUG
        
        ProgramOutputHandler::log("TextFileHandler.write() -> Failed to open file: \""
            + file_path + "\"", Frost::WARN);
        #endif

        return false;
    }

    // Add the content to the txt file.
    s_output_stream << s_buffer;

    s_output_stream.close();

    if(clear_buffer) s_buffer.clear();

    return true;
}

std::string TextFileHandler::fetch(std::string file_path)
{
    std::string fetched_content;

    std::ifstream& s_input_stream = _get_input_stream();
    std::stringstream& s_string_stream = _get_string_stream();

    // If the path does not exist.
    if(!FileSystemHandler::does_directory_exist(file_path))
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("TextFileHandler.fetch() -> Directory doesn't exist: \""
            + file_path + "\", returning empty string.", Frost::WARN);
        #endif

        return std::string {};
    }

    s_input_stream.open(file_path);

    // If the file failed to open.
    if(!s_input_stream.is_open())
    {
        #ifdef FROST_DEBUG
        
        ProgramOutputHandler::log("TextFileHandler.fetch() -> Failed to open file: \""
            + file_path + "\", empty string.", Frost::WARN);
        #endif

        return std::string {};
    }

    // Get the underlying data stream and read it into the stringstream.
    s_string_stream << s_input_stream.rdbuf();

    // Get the string value of the string stream.
    fetched_content = s_string_stream.str();

    s_string_stream.clear();

    s_input_stream.close();

    return fetched_content;
}


// Private

std::ofstream& TextFileHandler::_get_output_stream() {
    static std::ofstream output_stream;
    return output_stream;
}

std::ifstream& TextFileHandler::_get_input_stream() {
    static std::ifstream input_stream;
    return input_stream;
}

std::string& TextFileHandler::_get_buffer() {
    static std::string buffer;
    return buffer;
}

std::stringstream& TextFileHandler::_get_string_stream() {
    static std::stringstream string_stream;
    return string_stream;
}
