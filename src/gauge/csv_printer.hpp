#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <fstream>

#include "printer.hpp"
#include "benchmark.hpp"
#include "config_set.hpp"

namespace gauge
{

    class csv_printer : public printer
    {
    public:

        /// Create a new csv printer
        /// @param default_name The default output file name
        csv_printer(const std::string& default_name = "out.csv");

    public: // From printer

        /// @see printer::benchmark_result(const benchmark&,const table&)
        void benchmark_result(const benchmark& info,
            const tables::table& results);

        /// @see printer::end()
        void end();

        /// @see printer::set_options(po::variables_map&);
        void set_options(po::variables_map& options);

    private:

        void init_header(const benchmark& info, const tables::table& results);

        // void add_row();

    private:

        /// Store the filename of the output file
        std::string m_filename;

        /// Store the value separator
        std::string m_value_seperator;

        /// The output file stream
        std::ofstream m_out;

        /// The output table
        tables::table m_final;

    };

}
