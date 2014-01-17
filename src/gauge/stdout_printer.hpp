#pragma once

#include <tables/format.hpp>
#include "printer.hpp"

namespace gauge
{

    namespace po = boost::program_options;

    /// A console printer which outputs the benchmark results based on the
    /// provided formatter.
    class stdout_printer : printer
    {

    public: // from printer

        //
        stdout_printer();

        //
        void set_options(po::variables_map& options);

    private:
        typedef std::map<std::string, tables::format> formatter_map;

    private:

        tables::format m_format;
        static const formatter_map m_formatters;
    };
}
