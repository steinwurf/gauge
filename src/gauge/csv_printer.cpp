#include <ostream>

#include <boost/program_options.hpp>

#include <tables/csv_format.hpp>
#include <tables/table.hpp>

#include "csv_printer.hpp"
#include "file_printer.hpp"
#include "runner.hpp"

namespace gauge
{
    csv_printer::csv_printer(
        const std::string& default_filename)
        : file_printer("csv", "csv", default_filename)
    {
        // Add the filename option for this printer
        gauge::po::options_description options;

        auto default_value_seperator =
            gauge::po::value<std::string>()->default_value(",");

        options.add_options()
            ("csvseperator", default_value_seperator,
             "Set the value separator for the csv file writer");

        gauge::runner::instance().register_options(options);
    }

    void csv_printer::print_to_stream(std::ostream &s)
    {
        tables::table combined_results;
        for (auto i = m_tables.begin(); i != m_tables.end(); ++i)
        {
            combined_results.merge(*i);
        }

        tables::csv_format format;

        format.print(s, combined_results);
    }

    void csv_printer::set_options(po::variables_map& options)
    {
        file_printer::set_options(options);
        m_value_seperator = options["csvseperator"].as<std::string>();
    }
}

