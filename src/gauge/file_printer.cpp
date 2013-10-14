#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>
#include <tables/table.hpp>

#include "benchmark.hpp"
#include "file_printer.hpp"
#include "runner.hpp"

namespace gauge
{

    file_printer::file_printer(const std::string& filename_option,
        const std::string& description,
        const std::string& default_filename)
            : m_filename_option(filename_option)
    {
        // Add the filename option for this printer
        gauge::po::options_description options;

        auto default_filename_value = gauge::po::value<std::string>()->
            default_value(default_filename);

        options.add_options()
            (m_filename_option.c_str(), default_filename_value,
             description.c_str());

        gauge::runner::instance().register_options(options);
    }

    void file_printer::benchmark_result(const benchmark &info,
                                        const tables::table &results)
    {
        (void)info;
        m_tables.insert(m_tables.end(), results);
    }

    void file_printer::end()
    {
        std::ofstream result_file;
        result_file.open(m_filename, std::ios::trunc);
        print_to_stream(result_file);
        result_file.close();
    }

    void file_printer::set_options(po::variables_map& options)
    {
        m_filename = options[m_filename_option].as<std::string>();
        assert(!m_filename.empty());
    }
}
