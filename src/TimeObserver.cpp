#include <chrono>
#include <iomanip>

#include "TimeObserver.hpp"


// Static Members

std::ostringstream TimeObserver::s_out_str_stream;


// Public

std::string TimeObserver::get_date()
{
    reset_out_stream();

    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    s_out_str_stream << std::put_time(std::localtime(&time), "%y-%m-%d");

    return std::move(s_out_str_stream.str());
}

std::string TimeObserver::get_time() 
{
    reset_out_stream();

    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    s_out_str_stream << std::put_time(std::localtime(&time), "%I:%M:%S %p");

    return std::move(s_out_str_stream.str());
}


// Private

void TimeObserver::reset_out_stream()
{
    s_out_str_stream.str("");
}
