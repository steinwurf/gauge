#pragma once

#include <string>
#include <ostream>

#include "file_printer.hpp"

namespace gauge
{
    class csv_printer : public file_printer
    {
    public:
        /// Create a new csv printer
        /// @param default_filename The default name of the outputted file
        csv_printer(const std::string& default_filename = "out.csv");

    public: // From file_printer

        /// @see file_printer::print_to_stream(std::ostream &s)
        void print_to_stream(std::ostream &s);
    };
}
