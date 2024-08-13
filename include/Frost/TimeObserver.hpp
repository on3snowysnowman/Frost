/**
 * @file TimeObserver.hpp
 * @author Joel Height (on3snowysnowman@gmail.com)
 * @brief Contains a class that manages fetching local time and dates.
 * @version 0.1
 * @date 2024-08-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <sstream>

/** Static class that contains useful methods for fetching the current time and date. */
class TimeObserver
{

public:

    /** Gets the current date. Formatted YY-MM-DD. */
    static std::string get_date();
    
    /** Gets the current time. */
    static std::string get_time();

private:

    // Members

    static std::ostringstream s_out_str_stream;


    // Methods

    /** Resets the s_out_str_stream. */
    static void reset_out_stream();
};
