#include <filesystem>

#include "FileSystemHandler.hpp"


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

bool FileSystemHandler::copy_file(const char *target_file_path, const char *target_directory)
{
    // Check if the source file exists
    if (!does_directory_exist(target_file_path)) return false;

    // Construct the destination path
    std::filesystem::path destination = std::filesystem::path(target_directory) / std::filesystem::path(target_file_path).filename();

    try
    {
        // Copy the file to the destination
        std::filesystem::copy_file(target_file_path, destination, std::filesystem::copy_options::overwrite_existing);
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        return false;
    }

    return true;
}

bool FileSystemHandler::copy_file(const char *target_file_path, const char *target_directory, const char *new_file_name)
{
    // Check if the source file exists
    if (!does_directory_exist(target_file_path)) return false;

    // Construct the destination path with the new file name
    std::filesystem::path destination = std::filesystem::path(target_directory) / new_file_name;

    try
    {
        // Copy the file to the destination
        std::filesystem::copy_file(target_file_path, destination, std::filesystem::copy_options::overwrite_existing);
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        return false;
    }

    return true;
}


uintmax_t FileSystemHandler::get_file_size(std::string file_path)
{
    // If the file doesn't exist.
    if(!does_directory_exist(file_path)) return 0;

    return std::filesystem::file_size(file_path);
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
