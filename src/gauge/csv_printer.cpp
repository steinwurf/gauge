// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.


#include <tables/csv_format.hpp>
#include <tables/table.hpp>

#include <ostream>
#include <string>

#include "csv_printer.hpp"
#include "file_printer.hpp"
#include "runner.hpp"

namespace gauge
{
csv_printer::csv_printer(const std::string& default_filename) :
    file_printer("csv", default_filename)
{ }

void csv_printer::print_to_stream(std::ostream& s)
{
    tables::table combined_results;
    for (auto i = m_tables.begin(); i != m_tables.end(); ++i)
    {
        combined_results.merge(*i);
    }

    tables::csv_format format;

    format.print(s, combined_results);
}
}
