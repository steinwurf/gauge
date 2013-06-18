#include "python_printer.hpp"
#include "runner.hpp"
#include "pydict.hpp"

namespace gauge
{

    python_printer::python_printer()
    {

        // Add the filename option for this printer
        gauge::po::options_description options;

        auto default_name =
            gauge::po::value<std::string>()->default_value("out.py");

        options.add_options()
            ("pyfile", default_name,
             "Set the output name of the python printer");

        gauge::runner::instance().register_options(options);
    }

    void python_printer::benchmark_result(const benchmark &info,
                                          const table &results)
    {
        pydict benchmark_dict;

        benchmark_dict.add("unit", info.unit_text());
        benchmark_dict.add("benchmark", info.benchmark_name());
        benchmark_dict.add("testcase", info.testcase_name());

        if(info.has_configurations())
        {
            const auto& c = info.get_current_configuration();
            for(const auto& v : c)
            {
                benchmark_dict.add(v.first, v.second);
            }
        }

        for(const auto& r: results)
        {
            benchmark_dict.add(r.first, r.second.m_values);
        }

        m_list.add(benchmark_dict);
    }

    void python_printer::end()
    {
        m_out.open(m_filename, std::ios::trunc);
        m_out << "results = ";

        python_format f;
        f.print(m_out, m_list);
        m_out.close();
    }

    void python_printer::set_options(po::variables_map& options)
    {
        m_filename = options["pyfile"].as<std::string>();
        assert(!m_filename.empty());
    }
}

