
#include <tables/csv_format.hpp>
#include <tables/json_format.hpp>
#include <tables/python_format.hpp>

#include "runner.hpp"

#include "stdout_printer.hpp"

namespace gauge
{
    const stdout_printer::formatter_map stdout_printer::m_formatters {
        {"csv", tables::csv_format()},
        {"json", tables::json_format()},
        {"python", tables::python_format()}
    };

    stdout_printer::stdout_printer() :
        printer("stdout", false)
    {
        gauge::po::options_description options;

        options.add_options()(
            "stdout_formatter",
            po::value<std::string>(),
            "The format to use for the stdout printer");

        gauge::runner::instance().register_options(options);
    }

    void stdout_printer::set_options(po::variables_map& options)
    {
        formatter_map::key_type key =
            options["stdout_formatter"].as<formatter_map::key_type>();
        m_format = m_formatters.at(key);
    }
}
