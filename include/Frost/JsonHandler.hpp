#pragma once

#include <string>
#include <fstream>

#include "Json.hpp"

/** A static class that handles fetching and dumping of json objects to and from json files. */
class JsonHandler
{

public:
    
    /** Clears the json file at the passed file path, and places an empty json object inside. 
     * Returns true if the file was successfully cleared.
    */
    static bool clear(std::string file_path);

    /** Dumps the passed json object to the json file at the passed file path. Returns true if the 
     * json object was successfully dumped.
     * 
     * @param json_obj Object to dump
     * @param file_path Path to the json file to dump to.
     */
    static bool dump(json& json_obj, std::string file_path);

    /** Returns a json object filled with the contents of the json file at the passed file path. 
     * 
     * @param file_path Path to the json file to fetch from.
    */
    static json get(std::string file_path);

private:

    static std::fstream s_file_stream;

};
