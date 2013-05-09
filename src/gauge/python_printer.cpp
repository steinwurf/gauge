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

    void python_printer::benchmark_result(const benchmark &info,
                                          const table &results)
    {
        // pydict benchmark_dict;

        // benchmark_dict.add("testcase", info.testcase_name());
        // benchmark_dict.add("benchmark", info.benchmark_name());
        // benchmark_dict.add("unit", results.unit());
        // benchmark_dict.add("iterations", results.iterations());

        // if(info.has_configurations())
        // {
        //     const auto& c = info.get_current_configuration();
        //     for(const auto& v : c)
        //     {
        //         std::stringstream ss;
        //         v.second->print(ss, python_format());
        //         benchmark_dict.add(v.first, ss.str());
        //     }
        // }

        // for(const auto& r: results)
        // {
        //     benchmark_dict.add(r.first, r.second);
        // }

        // m_list.add(benchmark_dict);
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

