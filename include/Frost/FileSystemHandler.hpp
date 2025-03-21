#pragma once

#include <fstream>
#include <cstdint>
#include <vector>

/** 
 * @brief Full static method class for file system management. 
 * 
 * This includes the ability to create a directory, see what files are at a 
 * directory, and make and delete files. 
 * 
 * */
class FileSystemHandler
{
public:

    /** 
     * @brief Makes a directory at the passed path if it does not already 
     * exist. Returns true if the directory was created.
     * 
     * @param directory_path Path to create the directory.
     * 
     */
    static bool make_directory(std::string directory_path);

    /** 
     * @brief Creates a file at the passed path if it does not already exist. 
     * Returns true if the file was created. 
     * 
     * @param file_path Path to create the file at.
     * 
     */
    static bool make_file(std::string file_path);

    /** 
     * @brief Delete the directory and its contents at the passed path. 
     * 
     * Returns true if the directory wassuccessfully deleted. 
     * 
     * @param directory_path Path to the directory to delete. 
     * 
     * */
    static bool delete_directory(std::string directory_path);

    /** 
     * @brief Delete the file at the passed path. Returns true if the file was 
     * successfully deleted.
     * 
     * @param file_path Path to the file to delete.
     * 
     */
    static bool delete_file(std::string file_path);

    /**
     * @brief Returns true if the passed path is a valid directory.
     * 
     * @param directory_path Directory to check.
     * 
     */
    static bool does_directory_exist(std::string directory_path);

    /** 
     * @brief Returns the size in bytes of the file at the passed path. 
     * 
     * Returns 0 if the failed to be parsed. 
     * 
     */
    static uint64_t get_file_size(std::string file_path);

    /** 
     * @brief Returns the path of the current working directory. 
     * 
     */
    static std::string get_current_path();

    /** Returns a vector of file paths that are in the passed directory.
     * 
     * @param directory_path Path to check.
     * 
     */
    static std::vector<std::string> 
        get_files_at_directory(std::string directory_path);


private:

    static std::ofstream s_out_file_stream;
};