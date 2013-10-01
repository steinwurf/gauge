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

    /// A python result printer implementation. The purpose
    /// of this printer is to "dump" results to a .py file
    /// which then may be easily used in python scripts for
    /// plotting etc.
    class file_printer : public printer
    {
    public:

        /// Create a new python printer
        /// @param printer_name The name of the printer type
        /// @param extension The file extension of the result file
        /// @param default_filename The default name of the outputted file
        /// without the file extension
        file_printer(const std::string& printer_name,
                     const std::string& extension,
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
