#include <chrono>
#include <iomanip>

#include "TimeObserver.hpp"


// Static Members

std::ostringstream TimeObserver::s_out_str_stream;


// Public

std::string TimeObserver::get_local_date()
{
    reset_out_stream();

    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    s_out_str_stream << std::put_time(std::localtime(&time), "%y-%m-%d");

    return s_out_str_stream.str();
}

std::string TimeObserver::get_local_time() 
{
    reset_out_stream();

    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    s_out_str_stream << std::put_time(std::localtime(&time), "%I:%M:%S %p");

    return s_out_str_stream.str();
}

c_time_point TimeObserver::get_time_point()
{
    return std::chrono::high_resolution_clock::now();
}

double TimeObserver::calculate_interval_from_timepoints(
    std::chrono::time_point<std::chrono::system_clock> start, 
    std::chrono::time_point<std::chrono::system_clock> end)
{
    return std::chrono::duration<double, std::milli>(end - start).count();
}


// Private

void TimeObserver::reset_out_stream()
{
    s_out_str_stream.str("");
}
