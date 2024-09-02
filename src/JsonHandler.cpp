#include "JsonHandler.hpp"
#include "FileSystemHandler.hpp"

#ifdef FROST_DEBUG

#include "ProgramOutputHandler.hpp"
#endif

// Static Members

std::fstream JsonHandler::s_file_stream;


// Public

bool JsonHandler::clear(std::string file_path)
{
    // If the path does not exist.
    if(!FileSystemHandler::does_directory_exist(file_path))
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("JsonHelper.clear() -> Directory doesn't exist: \"" 
            + file_path + "\"", Frost::WARN);
        #endif

        return false;
    }

    s_file_stream.open(file_path, std::ios::in);

    // If the file failed to open.
    if(!s_file_stream.is_open())
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("JsonHelper.clear() -> Failed to open file: \"" 
            + file_path + "\"", Frost::WARN);
        #endif

        return false;
    }

    s_file_stream << json::object({});
    s_file_stream.close();

    return true;
}

bool JsonHandler::dump(json& json_obj, std::string file_path)
{
    // File is automatically created if it doesn't exist.

    s_file_stream.open(file_path, std::ios::out);

    // If the file failed to open.
    if(!s_file_stream.is_open())
    {
        #ifdef FROST_DEBUG
        ProgramOutputHandler::log("JsonHandler.dump() -> Failed to open file: \""
            + file_path + "\"", Frost::WARN);
        #endif
        
        return false;
    }

    s_file_stream << json_obj.dump(4);
    s_file_stream.close();
    return true;
}

json JsonHandler::get(std::string file_path)
{
    // If the path does not exist.
    if(!FileSystemHandler::does_directory_exist(file_path))
    {
        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("JsonHelper.get() -> Directory doesn't exist: \"" 
            + file_path + "\"", Frost::WARN);
        #endif
        return json{};
    }

    s_file_stream.open(file_path, std::ios::in);

    // If the file failed to open.
    if(!s_file_stream.is_open())
    {

        #ifdef FROST_DEBUG

        ProgramOutputHandler::log("JsonHelper.get() -> Failed to open file: \"" 
            + file_path + "\"", Frost::WARN);
        #endif
        
        return json{};
    }

    json json_obj;

    s_file_stream >> json_obj;
    s_file_stream.close();

    return json_obj;
}

// Private
