#include "ProgramOutputHandler.hpp"
#include "Fr_StringManip.hpp"
#include "TextFileHandler.hpp"


// Static Members

std::string ProgramOutputHandler::s_output_file_path = "OutputLog.txt";

// Public

void ProgramOutputHandler::change_output_file_path(std::string new_path)
{
    s_output_file_path = std::move(new_path);
}

bool ProgramOutputHandler::clear_output_file()
{
    return TextFileHandler::clear_file(s_output_file_path);
}

bool ProgramOutputHandler::log(std::string content, Frost::OUTPUT_SEVERITY out_severity)
{
    switch(out_severity)
    {
        case Frost::LOG:

            TextFileHandler::add_to_buffer("[LOG] ");
            break;

        case Frost::WARN:

            TextFileHandler::add_to_buffer("[WARN] ");
            break;

        case Frost::ERR:   

            TextFileHandler::add_to_buffer("[ERR] ");
            break;
    }

    // Trim the output so that each line does not exceed 90 characters
    Frost::configure_string_with_line_limit(content, 90);

    TextFileHandler::add_to_buffer(content);

    return TextFileHandler::write(s_output_file_path, Frost::APPEND);
}
