
#include <tables/csv_format.hpp>
#include <tables/json_format.hpp>
#include <tables/python_format.hpp>

#include "runner.hpp"

#include "stdout_printer.hpp"

namespace gauge
{
    stdout_printer::stdout_printer() :
        printer("stdout", false)
    {
        m_formatters.insert(std::make_pair("csv",
            std::shared_ptr<tables::format>(new tables::csv_format())));
        m_formatters.insert(std::make_pair("json",
            std::shared_ptr<tables::format>(new tables::json_format())));
        m_formatters.insert(std::make_pair("python",
            std::shared_ptr<tables::format>(new tables::python_format())));

        gauge::po::options_description options;

        options.add_options()(
            "stdout_formatter",
            po::value<std::string>()->default_value(""),
            "The format to use for the stdout printer");

        gauge::runner::instance().register_options(options);
    }

    void stdout_printer::benchmark_result(const benchmark &info,
                                          const tables::table &results)
    {
        tables::table output = results;
        if(info.has_configurations())
        {
            const auto& c = info.get_current_configuration();
            for(const auto& v : c)
            {
                output.add_const_column(v.first, v.second);
            }
        }
        m_tables.insert(m_tables.end(), output);
    }

    void stdout_printer::end()
    {
        m_formatters.at(m_format_key)->print(std::cout, m_tables);
    }

    void stdout_printer::set_options(po::variables_map& options)
    {
        printer::set_options(options);
        if (m_enabled)
        {
            m_format_key =
                options["stdout_formatter"].as<formatter_map::key_type>();
            if(!m_formatters.count(m_format_key))
            {
                throw std::runtime_error(
                    m_format_key + " is not a valid format.");
            }
        }
    }
}
