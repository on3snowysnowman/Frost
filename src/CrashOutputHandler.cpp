/**
 * @file ExceptionHandler.cpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Single class implementation
 * @version 0.1
 * @date 01-24-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "CrashOutputHandler.hpp"
#include "TextFileHandler.hpp"
#include "TimeObserver.hpp"
#include "Fr_StringManip.hpp"


// Static Members

std::string CrashOutputHandler::s_output_file_path = "CrashLog.txt";


// Public

void CrashOutputHandler::set_output_file_path(const std::string& new_file_path)
    { s_output_file_path = new_file_path; }

void CrashOutputHandler::clear_output_file()
{
    TextFileHandler::clear_file(s_output_file_path);
}

#if defined(__clang__) || defined(__GNUC__)

void CrashOutputHandler::output_crash(const std::string& message, const std::string& func)
{
    std::string full_output = TimeObserver::get_local_date() + " @ " + 
        TimeObserver::get_local_time() + " : " + func + " -> " + message + '\n';

    TextFileHandler::add_to_buffer(full_output);
    TextFileHandler::write(s_output_file_path);
}

#else

void CrashOutputHandler::output_crash(const std::string& message, 
    const std::string file_name, int file_line, const std::string& func)
{
    std::string full_output = TimeObserver::get_local_date() + " @ " + 
        TimeObserver::get_local_time() + " : " + func + " (" + file_name
        + ":" + std::to_string(file_line) + ") -> " + message + '\n';

    TextFileHandler::add_to_buffer(full_output);
    TextFileHandler::write(s_output_file_path);
}

#endif
