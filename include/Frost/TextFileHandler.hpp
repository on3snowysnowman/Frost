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
 * The TextFileHandler uses a buffering system for text, to remove the possibily of a file being
 * opened and closed many times per frame if many strings are added invidividually. When the 
 * add_to_buffer() method is used, the contents of the string are added to the buffer. Only when the 
 * write() method is called will the buffered strings be emplaced in a file.
 * 
 * When calling the write() method, the user must specifiy the target behavior for writing to 
 * the file. If the buffer should be added to the content at the target txt file, use the APPEND
 * WriteType. Otherwise, the OVERWRITE WriteType will overwrite the contents of the file.
 * 
 * Lastly, use the fetch() method to get the contents at a text file.
 */
class TextFileHandler
{

public:

    /** Concatinate a string to the buffered content. Note that this does not write it to a file,
     * it simply adds this string to the buffer.
     * 
     * @param str Content to add to the buffer.
     */
    static void add_to_buffer(std::string str);

    /** Clears the buffer of all added text. */
    static void clear_buffer();

    /** Clears the contents of a file. Returns true if the clear was successful.
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
     * @attention The contents of the file are stored in a heap allocated string, the user is 
     * responsible for managing this memory.
     * 
     * @param file_path Path to the file.
     */
    static std::string fetch(std::string file_path);


private:

    // Members
    
    // Buffered content that will be written to a file on write() call.
    static std::string s_buffer;

    // Used for storing the converted string value of the s_string_stream.
    static std::string s_fetched_content;

    static std::ifstream s_input_stream;

    static std::ofstream s_output_stream;

    static std::ostringstream s_string_stream;

};