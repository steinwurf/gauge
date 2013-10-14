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
        /// @param filename_option The name of option for specifying the name of
        /// the output file
        /// @param description The description for the output filename option
        /// @param default_filename The default name of the outputted file
        /// without the file extension
        file_printer(const std::string& filename_option,
            const std::string& description,
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
