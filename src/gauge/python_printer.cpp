#include <ostream>
#include <tables/python_format.hpp>

#include "file_printer.hpp"
#include "python_printer.hpp"
#include "runner.hpp"

namespace gauge
{

    python_printer::python_printer() : file_printer("python", "py")
    { }

    void python_printer::print_to_stream(std::ostream &s)
    {
        tables::python_format format;
        format.print(s, m_tables);
    }
}

