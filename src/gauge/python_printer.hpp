#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <fstream>

#include "printer.hpp"
#include "benchmark.hpp"
#include "config_set.hpp"
#include "python_format.hpp"
#include "pylist.hpp"

namespace gauge
{

    /// A python result printer implementation. The purpose
    /// of this printer is to "dump" results to a .py file
    /// which then may be easily used in python scripts for
    /// plotting etc.
    class python_printer : public printer
    {
    public:

        /// Create a new python printer
        python_printer();

    public: // From printer

        /// @see printer::benchmark_result(const benchmark&,const table&)
        void benchmark_result(const benchmark& info, const table& results);

        /// @see printer::end()
        void end();

        /// @see printer::set_options(po::variables_map&);
        void set_options(po::variables_map& options);

    private:

        /// Results are printed as a python list using this
        /// pylist
        pylist m_list;

        /// Store the filename of the output file
        std::string m_filename;

        /// The output file stream
        std::ofstream m_out;
    };

}



