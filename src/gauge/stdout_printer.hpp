#pragma once

#include <tables/format.hpp>
#include <tables/table.hpp>

#include "benchmark.hpp"
#include "printer.hpp"

namespace gauge
{

    namespace po = boost::program_options;

    /// A console printer which outputs the benchmark results based on the
    /// provided formatter.
    class stdout_printer : public printer
    {

    public: // from printer

        ///
        stdout_printer();

        ///
        void set_options(po::variables_map& options);

        ///
        void benchmark_result(const benchmark &info,
                              const tables::table &results);

        ///
        void end();

    private:

        ///
        typedef std::map<std::string, std::shared_ptr<tables::format> >
            formatter_map;

    private:

        ///
        formatter_map::key_type m_format_key;

        ///
        static const formatter_map m_formatters;

        /// The output tables
        std::vector<tables::table> m_tables;
    };
}
