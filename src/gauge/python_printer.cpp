#include <ostream>
#include <tables/python_format.hpp>

#include "file_printer.hpp"
#include "python_printer.hpp"
#include "runner.hpp"

namespace gauge
{
    python_printer::python_printer(const std::string& default_filename)
        : file_printer("pyfile", "Set the output name of the python printer",
            default_filename)
    { }

    void python_printer::print_to_stream(std::ostream &s)
    {
        tables::python_format format;
        format.print(s, m_tables);
    }
}
