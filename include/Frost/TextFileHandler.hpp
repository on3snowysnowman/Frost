#pragma once

#include <fstream>
#include <string>

namespace Frost
{
    // Behavior type for writing to files.
    enum WriteType
    {
        APPEND,
        OVERWRITE
    };
};


/** @brief Static class that handles writing to and fetching from txt files.
 * 
 * The TextFileHandler uses a buffering system for text, to remove the possibly of a file being
 * opened and closed many times per frame if many strings are added individually. When the 
 * add_to_buffer() method is used, the contents of the string are added to the buffer. Only when the 
 * write() method is called will the buffered strings be emplaced in a file.
 * 
 * When calling the write() method, the user must specify the target behavior for writing to 
 * the file. If the buffer should be added to the content at the target txt file, use the APPEND
 * WriteType. Otherwise, the OVERWRITE WriteType will overwrite the contents of the file.
 * 
 * Lastly, use the fetch() method to get the contents at a text file.
 */
class TextFileHandler
{

public:

    /** Concatenate a string to the buffered content. Note that this does not write it to a file,
     * it simply adds this string to the buffer.
     * 
     * @param str Content to add to the buffer.
     */
    static void add_to_buffer(std::string str);

    /** Clears the buffer of all added text. */
    static void clear_buffer();

    /** Clears the contents of a file. If the file doesn't exist, an empty one is created. 
     * Returns true if the clear (or creation) was successful.
     * 
     * @param file_path Path to the file.
    */
    static bool clear_file(std::string file_path);

    /** Writes the contents of the buffer to a file. Using WriteType APPEND adds the buffer to
     * the end of the contents at the file, while OVERWRITE replaces it. The buffer is cleared by 
     * default per each write() call, to disable this set the clear_buffer parameter to false.
     * Returns true if the write was successful.
     * 
     * @param file_path Path to the file.
     * @param write_type Specifier for file writing behavior. Default is OVERWRITE.
     * @param clear_buffer Whether to clear the buffer. Default to true.
    */
    static bool write(std::string file_path, Frost::WriteType write_type = Frost::OVERWRITE, 
        bool clear_buffer = true);

    /** Fetches and returns the contents of a txt file at the path. 
     * 
     * @param file_path Path to the file.
     */
    static std::string fetch(std::string file_path);


private:

    // Methods

    /** Construct on First Use function for the input stream. Prevents static fiasco. */
    static std::ifstream& _get_input_stream();

    /** Construct on First Use function for the output stream. Prevents static fiasco. */
    static std::ofstream& _get_output_stream();

    /** Construct on First Use function for the buffer. Prevents static fiasco. The buffer contains
     * buffered content that will be written to a file on the next "write()" call. */
    static std::string& _get_buffer();

    /** Construct on First Use function for the string stream. Prevents static fiasco. The string
     * stream is used for pulling in data from the input stream buffer. */
    static std::stringstream& _get_string_stream();
};