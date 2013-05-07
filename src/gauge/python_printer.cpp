#include "python_printer.hpp"
#include "runner.hpp"

namespace gauge
{
    pylist::pylist()
        : m_first(true)
    {}

    std::string pylist::str() const
    {
        std::string list = "[" + m_out.str() + "]";
        return list;
    }

    void pylist::clear()
    {
        m_first = true;
        m_out.str("");
    }


    pydict::pydict()
        : m_first(true)
    {}

    void pydict::clear()
    {
        m_first = true;
        m_out.str("");
    }

    std::string pydict::str() const
    {
        return "{" + m_out.str() + "}";
    }


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

    void python_printer::start_benchmark()
    { }

    void python_printer::benchmark_result(const benchmark &info,
                                          const results &result)
    {
        pydict benchmark_dict;

        benchmark_dict.add("testcase", info.testcase_name());
        benchmark_dict.add("benchmark", info.benchmark_name());
        benchmark_dict.add("unit", info.unit_text());

        if(info.has_configurations())
        {
            benchmark_dict.add("config", info.get_current_configuration());
        }

        benchmark_dict.add("data", result.m_results);
        benchmark_dict.add("iterations", result.m_iterations);

        m_list.add(benchmark_dict);
    }

    void python_printer::benchmark_result(uint32_t runs,
                                          const benchmark& info,
                                          const std::vector<table>& results)
    {
        pydict benchmark_dict;

        benchmark_dict.add("testcase", info.testcase_name());
        benchmark_dict.add("benchmark", info.benchmark_name());

        if(info.has_configurations())
        {
            benchmark_dict.add("config", info.get_current_configuration());
        }

        pydict table_dict;
        for(const auto& t : results)
        {
            for(const auto& r: t)
            {
                pydict result_dict;
                result_dict.add("unit", t.unit());
                result_dict.add("iterations", t.iterations());
                result_dict.add("values", r.second);

                table_dict.add(r.first, result_dict);
            }
        }

        benchmark_dict.add("result", table_dict);

        m_list.add(benchmark_dict);


    }


    void python_printer::end_benchmark()
    {
        m_out.open(m_filename, std::ios::trunc);
        m_out << "results = ";
        pyprint(m_out, m_list);
        m_out.close();
    }

    void python_printer::set_options(po::variables_map& options)
    {
        m_filename = options["pyfile"].as<std::string>();
        assert(!m_filename.empty());
    }
}

