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
#include <chrono>


typedef std::chrono::time_point<std::chrono::system_clock> c_time_point;

/** Static class that contains useful methods for fetching the current time and date, as well
 * as returning a high resolution time_point from chrono and returning the interval of two time 
 * points, allowing for high precision time measuring. */
class TimeObserver
{

public:

    /** Gets the local date (YY-MM-DD). */
    static std::string get_local_date();
    
    /** Gets the local time (Hours:Minutes). */
    static std::string get_local_time();

    /** Returns a std::chrono::time_point of the current time. */
    static  c_time_point get_time_point();

    /** Returns a high precision value of the time interval in miliseconds between two c_time_points, 
     * from $start to $end.
     *
     * @param start Starting time point.
     * @param end Ending time point. */
    static double calculate_interval_from_timepoints(
            std::chrono::time_point<std::chrono::system_clock> start, 
            std::chrono::time_point<std::chrono::system_clock> end);

private:

    // Members

    static std::ostringstream s_out_str_stream;


    // Methods

    /** Resets the s_out_str_stream. */
    static void reset_out_stream();
};
