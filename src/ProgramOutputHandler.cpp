#include "ProgramOutputHandler.h"
#include "Fr_StringManip.h"


// Static Members

std::string ProgramOutputHandler::s_output_file_path = "OutputLog.txt";

std::fstream ProgramOutputHandler::s_file_stream;

// Public

void ProgramOutputHandler::change_output_file_path(std::string new_path)
{
    s_output_file_path = std::move(new_path);
}

bool ProgramOutputHandler::clear_output_file()
{
    s_file_stream.open(s_output_file_path, std::ios::out);

    // Failed to open file
    if(!s_file_stream.is_open()) return false;

    s_file_stream.close();
    return true;
}

bool ProgramOutputHandler::log(std::string content, Frost::OUTPUT_SEVERITY out_severity)
{
    s_file_stream.open(s_output_file_path, std::ios::app);

    // Failed to open file
    if(!s_file_stream.is_open()) return false;

    switch(out_severity)
    {
        case Frost::LOG:

            s_file_stream << "[LOG] ";
            break;

        case Frost::WARN:

            s_file_stream << "[WARN] ";
            break;

        case Frost::ERR:   

            s_file_stream << "[ERR] ";
            break;
    }

    // Trim the output so that each line does not exceed 90 characters
    Frost::configure_string_with_line_limit(content, 90);

    s_file_stream << content << '\n';

    s_file_stream.close();
    return true;
}
