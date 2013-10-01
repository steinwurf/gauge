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
    file_printer::file_printer(const std::string& printer_name,
        const std::string& extension,
        const std::string& default_filename)
    {
        std::stringstream default_filename_ss;
        default_filename_ss << default_filename << "." << extension;

        std::stringstream filename_option_ss;
        filename_option_ss << extension << "file";
        m_filename_option = filename_option_ss.str();

        std::stringstream filename_option_desc_ss;
        filename_option_desc_ss << "Set the output name of the "
                                << printer_name << " printer";

        // Add the filename option for this printer
        gauge::po::options_description options;

        auto default_filename_value =
            gauge::po::value<std::string>()->default_value(
                default_filename_ss.str());

        options.add_options()
            (m_filename_option.c_str(), default_filename_value,
             filename_option_desc_ss.str().c_str());

        gauge::runner::instance().register_options(options);
    }

    void file_printer::benchmark_result(const benchmark &info,
                                        const tables::table &results)
    {
        tables::table r = results;
        if(info.has_configurations())
        {
            const auto& c = info.get_current_configuration();
            for(const auto& v : c)
            {
                r.add_column(v.first, v.second);
            }
        }

        m_tables.insert(m_tables.end(), r);
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

