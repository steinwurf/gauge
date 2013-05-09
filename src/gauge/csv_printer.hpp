#ifndef GAUGE_CSV_PRINTER_HPP
#define GAUGE_CSV_PRINTER_HPP

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

        /// Constructor
        csv_printer();

        /// Destructor
        ~csv_printer();

    public: // From printer

        /// @see printer::benchmark_result(const benchmark&,const table&)
        void benchmark_result(const benchmark& info, const table& results);

        /// @see printer::end_benchmark()
        void end_benchmark();

        /// @see printer::set_options(po::variables_map&);
        void set_options(po::variables_map& options);

    private:

        void init_header(const benchmark& info, const table& results);

        void add_row();

    private:

        struct impl;
        std::unique_ptr<impl> m_impl;

    };

}

#endif

