#include "python_printer.hpp"

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


    python_printer::python_printer(const std::string &filename)
        : m_filename(filename)
    { }

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

    void python_printer::end_benchmark()
    {
        m_out.open(m_filename, std::ios::trunc);
        m_out << "results = ";
        pyprint(m_out, m_list);
        m_out.close();
    }
}

