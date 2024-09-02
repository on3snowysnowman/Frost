#pragma once

#include <vector>
#include <fstream>
#include <cstdio>

/** A static class that contains useful methods for file system management. This includes the 
 * ability to create a directory, see what files are at a directory, and make and delete files. */
class FileSystemHandler
{
public:

    /** Makes a directory at the passed path if it does not already exist. Returns true if the 
     * directory was created.
     * 
     * @param directory_path Path to create the directory.
     */
    static bool make_directory(std::string directory_path);

    /** Creates a file at the passed path if it does not already exist. Returns true if the 
     * file was created. 
     * 
     * @param file_path Path to create the file at.
     */
    static bool make_file(std::string file_path);

    /** Delete the directory and its contents at the passed path. Returns true if the directory was
     * successfully deleted. 
     * 
     * @param directory_path Path to the directory to delete. 
     * */
    static bool delete_directory(std::string directory_path);

    /** Delete the file at the passed path. Returns true if the file was successfully deleted.
     * 
     * @param file_path Path to the file to delete.
     */
    static bool delete_file(std::string file_path);

    /**
     * Returns true if the passed path is a valid directory.
     * 
     * @param directory_path Directory to check.
    */
    static bool does_directory_exist(std::string directory_path);

    /** Returns the path of the current working directory. */
    static std::string get_current_path();

    /** Returns a vector of file paths that are in the passed directory.
     * 
     * @param directory_path Path to check.
     */
    static std::vector<std::string> 
        get_files_at_directory(std::string directory_path);


private:

    static std::ofstream s_out_file_stream;
};