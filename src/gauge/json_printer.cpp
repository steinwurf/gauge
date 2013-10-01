#include <ostream>
#include <tables/json_format.hpp>

#include "file_printer.hpp"
#include "json_printer.hpp"
#include "runner.hpp"

namespace gauge
{

    json_printer::json_printer() : file_printer("json", "json")
    { }

    void json_printer::print_to_stream(std::ostream &s)
    {
        tables::json_format format;
        format.print(s, m_tables);
    }
}
