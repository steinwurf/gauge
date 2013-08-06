#include "csv_printer.hpp"
#include "runner.hpp"

#include <boost/spirit/home/support/detail/hold_any.hpp>

namespace gauge
{
    struct csv_printer::impl
    {
        /// Store the filename of the output file
        std::string m_filename;

        /// Store the value separator
        std::string m_value_seperator;

        /// The output file stream
        std::ofstream m_out;

        /// The output table
        table m_final;
    };

    csv_printer::csv_printer(const std::string& default_name)
        : m_impl(new csv_printer::impl())
    {
        // Add the filename option for this printer
        gauge::po::options_description options;

        auto output_name =
            gauge::po::value<std::string>()->default_value(default_name);

        options.add_options()
            ("csvfile", output_name,
             "Set the output name of the csv printer");

        auto default_value_seperator =
            gauge::po::value<std::string>()->default_value(",");

        options.add_options()
            ("csvseperator", default_value_seperator,
             "Set the value separator for the csv file writer");

        gauge::runner::instance().register_options(options);
    }

    csv_printer::~csv_printer()
    {
    }

    void csv_printer::benchmark_result(const benchmark &info,
                                       const table &results)
    {
        table r = results;
        r.add_column("unit", info.unit_text());
        r.add_column("benchmark", info.benchmark_name());
        r.add_column("testcase", info.testcase_name());

        if(info.has_configurations())
        {
            const auto& c = info.get_current_configuration();
            for(const auto& v : c)
            {
                r.add_column(v.first, v.second);
            }
        }

        m_impl->m_final.merge(r);
    }

    void csv_printer::end()
    {
        assert(m_impl);

        m_impl->m_out.open(m_impl->m_filename, std::ios::trunc);
        m_impl->m_final.print(m_impl->m_out, format(),
                              m_impl->m_value_seperator);
        m_impl->m_out.close();
    }

    void csv_printer::set_options(po::variables_map& options)
    {
        m_impl->m_filename = options["csvfile"].as<std::string>();
        m_impl->m_value_seperator = options["csvseperator"].as<std::string>();
        assert(!m_impl->m_filename.empty());
    }
}

