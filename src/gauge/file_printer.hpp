// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <vector>
#include <ostream>

#include <boost/program_options.hpp>
#include <tables/table.hpp>

#include "printer.hpp"
#include "benchmark.hpp"

namespace gauge
{
    class file_printer : public printer
    {
    public:

        /// Create a new file printer
        /// @param name The name of the file printer.
        /// @param default_filename The default name of the outputted file
        /// (with file extension)
        file_printer(const std::string& name,
            const std::string& default_filename);

    public: // From printer

        /// @see printer::benchmark_result(const benchmark&,const table&)
        virtual void benchmark_result(const benchmark& info,
            const tables::table& results);

        /// @see printer::end()
        virtual void end();

        /// @see printer::set_options(po::variables_map&);
        virtual void set_options(po::variables_map& options);

    public:

        /// Prints data to a stream. Used in the file_printer::end method.
        /// @param s Steam to write to.
        virtual void print_to_stream(std::ostream &s) = 0;

    protected:

        /// Store the filename of the output file
        std::string m_filename;

        /// Store the name of the filename option
        std::string m_filename_option;

        /// The output tables
        std::vector<tables::table> m_tables;

    };

}
