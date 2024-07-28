#include "JsonHandler.hpp"
#include "ProgramOutputHandler.hpp"


// Static Members

std::fstream JsonHandler::s_file_stream;


// Public

bool JsonHandler::clear(std::string file_path)
{
    // If the path does not exist.
    if(!FileSystemHandler::does_directory_exist(file_path))
    {
        ProgramOutputHandler::log("JsonHelper.clear() -> Directory doesn't exist: \"" 
            + file_path + "\"", Frost::WARN);
        return false;
    }

    s_file_stream.open(file_path, std::ios::in);

    // If the file failed to open.
    if(!s_file_stream.is_open())
    {
        ProgramOutputHandler::log("JsonHelper.clear() -> Failed to open file: \"" 
            + file_path + "\"", Frost::WARN);
        return false;
    }

    s_file_stream << json::object({});
    s_file_stream.close();

    return true;
}

bool JsonHandler::dump(json& json_obj, std::string file_path)
{
    // If the file doesn't exist.
    if(!FileSystemHandler::does_directory_exist(file_path))
    {
        // Create the file.
        FileSystemHandler::make_file(file_path);

        ProgramOutputHandler::log("JsonHandler.dump() -> File: \"" + file_path + "\" doesn't "
            "exist, creating file.");
    }

    s_file_stream.open(file_path, std::ios::out);

    // If the file failed to open.
    if(!s_file_stream.is_open())
    {
        ProgramOutputHandler::log("JsonHandler.dump() -> Failed to open file: \""
            + file_path + "\"", Frost::WARN);
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
        ProgramOutputHandler::log("JsonHelper.get() -> Directory doesn't exist: \"" 
            + file_path + "\"", Frost::WARN);
        return json{};
    }

    s_file_stream.open(file_path, std::ios::in);

    // If the file failed to open.
    if(!s_file_stream.is_open())
    {
        ProgramOutputHandler::log("JsonHelper.get() -> Failed to open file: \"" 
            + file_path + "\"", Frost::WARN);
        return json{};
    }

    json json_obj;

    s_file_stream >> json_obj;
    s_file_stream.close();

    return json_obj;
}

// Private
