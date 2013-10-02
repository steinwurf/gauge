#include <ostream>
#include <tables/json_format.hpp>

#include "file_printer.hpp"
#include "json_printer.hpp"
#include "runner.hpp"

namespace gauge
{

    json_printer::json_printer(const std::string& default_filename)
        : file_printer("jsonfile", "Set the output name of the json printer",
            default_filename)
    { }

    void json_printer::print_to_stream(std::ostream &s)
    {
        tables::json_format format;
        format.print(s, m_tables);
    }
}
