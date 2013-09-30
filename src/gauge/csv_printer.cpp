#include "csv_printer.hpp"
#include "runner.hpp"

#include <boost/spirit/home/support/detail/hold_any.hpp>

namespace gauge
{
    csv_printer::csv_printer(const std::string& default_name)
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

    void csv_printer::benchmark_result(const benchmark &info,
                                       const tables::table &results)
    {
        tables::table r = results;
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

        m_final.merge(r);
    }

    void csv_printer::end()
    {

        m_out.open(m_filename, std::ios::trunc);
        m_final.print(m_out, tables::format(),
                              m_value_seperator);
        m_out.close();
    }

    void csv_printer::set_options(po::variables_map& options)
    {
        m_filename = options["csvfile"].as<std::string>();
        m_value_seperator = options["csvseperator"].as<std::string>();
        assert(!m_filename.empty());
    }
}

