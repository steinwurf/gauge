#include "csv_printer.hpp"
#include "runner.hpp"

#include <boost/spirit/home/support/detail/hold_any.hpp>

namespace gauge
{
    struct csv_printer::impl
    {
        /// Store the filename of the output file
        std::string m_filename;

        /// The output file stream
        std::ofstream m_out;

        /// The output table
        table m_final;
    };

    csv_printer::csv_printer()
        : m_impl(new csv_printer::impl())
    {
        // Add the filename option for this printer
        gauge::po::options_description options;

        auto default_name =
            gauge::po::value<std::string>()->default_value("out.csv");

        options.add_options()
            ("csvfile", default_name,
             "Set the output name of the csv printer");

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

    void csv_printer::end_benchmark()
    {
        assert(m_impl);

        m_impl->m_out.open(m_impl->m_filename, std::ios::trunc);
        m_impl->m_final.print(m_impl->m_out);
        m_impl->m_out.close();
    }

    void csv_printer::set_options(po::variables_map& options)
    {
        m_impl->m_filename = options["csvfile"].as<std::string>();
        assert(!m_impl->m_filename.empty());
    }
}

