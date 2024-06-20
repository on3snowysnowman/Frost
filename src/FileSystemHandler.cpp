#include "FileSystemHandler.h"


// Static Members

std::ofstream FileSystemHandler::s_out_file_stream;


// Public

bool FileSystemHandler::make_directory(std::string directory_path)
{
    // If the directory already exists.
    if(does_directory_exist(directory_path)) return false;

    return std::filesystem::create_directory(directory_path);
}

bool FileSystemHandler::make_file(std::string file_path)
{
    // If the file already exists.
    if(does_directory_exist(file_path)) return false;

    s_out_file_stream.open(file_path);

    // If the file failed to open.
    if(!s_out_file_stream.is_open()) return false;

    // By simply opening the file, it has been created.

    s_out_file_stream.close();

    return true;
}

bool FileSystemHandler::delete_directory(std::string directory_path)
{
    // If the directory does not exist
    if(!does_directory_exist(directory_path)) return false;

    try{
        std::filesystem::remove_all(directory_path);
    }   
    
    // Failed to delete directory
    catch(const std::filesystem::filesystem_error& err)
    {
        return false;
    }

    return true;
}

bool FileSystemHandler::delete_file(std::string file_path)
{
    // If the file does not exist.
    if(!does_directory_exist(file_path)) return false;

    return std::remove(file_path.c_str()) == 0;
}

bool FileSystemHandler::does_directory_exist(std::string directory_path)
{
    return std::filesystem::exists(directory_path);
}

std::string FileSystemHandler::get_current_path()
{
    return std::filesystem::current_path().string();
} 

std::vector<std::string> FileSystemHandler::get_files_at_directory(std::string directory_path)
{
    std::vector<std::string> paths;

    // Iterate through each file in the directory
    for(const auto& file :
        std::filesystem::directory_iterator(directory_path))
    {
        paths.push_back(file.path().string());
    }

    return paths;
}
