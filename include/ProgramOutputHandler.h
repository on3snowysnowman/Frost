#pragma once

#include <cstdint>
#include <string>
#include <fstream>


enum OUTPUT_SEVERITY : uint8_t
{
    LOG,
    WARN,
    ERR
};


/** Allows for output from the program to a text file. Can be used for debugging, general or 
 * general output from the program. The output file should be cleared per each execution of the
 * program using the clear_output_file, otherwise new output will be appended onto old execution 
 * output. */
class ProgramOutputHandler
{

public:

    /** Changes the output file path to a new path. */
    static void change_output_file_path(std::string new_path);
    
    /** Clears the output file. Returns true if the clear was sucessful. */
    static bool clear_output_file();

    /** Logs the content to the output file with the output severity as a specifier. Returns 
     * true if the output was successful. The string is passed by reference, so the content is 
     * directly modified and no copy is made.
     * 
     * @param content Content to output. Passed by reference.
     * @param out_severity The severity of the output, changes the text identifier that is placed
     *  right before the output in the output file. */
    static bool log(std::string content, OUTPUT_SEVERITY out_severity = LOG);

private:

    // Members

    static std::string s_output_file_path; // Path to the output file

    static std::fstream s_file_stream;


    // Methods

    /** Trims the output */
    void _trim_output(std::string& output, uint8_t character_limit);
};
