/**
 * @file ExceptionHandler.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class declaration.
 * @version 0.1
 * @date 01-24-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <string>


/** 
 * @brief Static class that handles dumping exception context to the output file.
 * 
 * By default, the output file is 'CrashLog.txt'. This can be changed through the 
 * `set_output_file_path` method.
 * 
 */
class CrashOutputHandler
{

public:

    /** 
     * @brief Sets the path of the output file to a new path.
     * 
     * @param new_file_path New path to set.
     * 
    */
    static void set_output_file_path(const std::string& new_file_path);

    /**
     * @brief Clears the output file of any content.
     */
    static void clear_output_file();

    // User verbose function output if the using GNU or Clang.
    #if defined(__clang__) || defined(__GNUC__)

    static void output_crash(const std::string& message, const std::string& func);

    #else

    static void output_crash(const std::string& message, const std::string file_name, 
        int file_line, const std::string& func);

    #endif

private:

    // Path to the error output txt file.
    static std::string s_output_file_path;
};

#if defined(__clang__) || defined(__GNUC__)

#define OUTPUT_CRASH_DETAILS(message)\
    CrashOutputHandler::output_crash(message, __PRETTY_FUNCTION__)

#else 

#define OUTPUT_CRASH_DETAILS(message)\
    CrashOutputHandler::output_crash(message, __FILE__, __LINE__, __func__)

#endif