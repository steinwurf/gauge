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

    // void csv_printer::add_row()
    // {
    //     for(auto& i : m_impl->m_csv)
    //     {
    //         i.second.resize(i.second.size() + 1);
    //     }
    // }

    void csv_printer::benchmark_result(const benchmark &info,
                                       const table &results)
    {
        m_impl->m_final.merge(results);

        // results.print(std::cout);
        // init_header(info, results);
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

    void csv_printer::init_header(const benchmark& info, const table& results)
    {
        // if(info.has_configurations())
        // {
        //     const auto& c = info.get_current_configuration();
        //     for(const auto& v : c)
        //     {
        //         m_configs.insert(v.first);
        //     }
        // }

        // for(const auto& r: results)
        // {
        //     m_results.insert(r.first);
        // }

    }

    void csv_printer::end_benchmark()
    {

        m_impl->m_final.print(std::cout);
        // std::stringstream header;
        // header << "benchmark" << ","
        //        << "testcase" << ","
        //        << "unit" << ","
        //        << "runs" << ","
        //        << "iterations";

        // for(const auto& c : m_configs)
        // {
        //     header << "," << c;
        // }

        // for(const auto& r : m_results)
        // {
        //     header << "," << r;
        // }

        // std::cout << "HEADER " << header.str() << std::endl;

        // m_out.open(m_filename, std::ios::trunc);
        // m_out << "results = ";
        // pyprint(m_out, m_list);
        // m_out.close();
    }

    void csv_printer::set_options(po::variables_map& options)
    {
        // m_filename = options["csvfile"].as<std::string>();
        // assert(!m_filename.empty());
    }
}

