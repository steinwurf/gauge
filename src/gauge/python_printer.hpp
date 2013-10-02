#pragma once

#include <ostream>

#include "file_printer.hpp"


namespace gauge
{
    /// A python result printer implementation. The purpose
    /// of this printer is to "dump" results to a .py file
    /// which then may be easily used in python scripts for
    /// plotting etc.
    class python_printer : public file_printer
    {
    public:
        /// Create a new json printer
        /// @param default_filename The default name of the outputted file
        python_printer(const std::string& default_filename = "out");

    public: // From file_printer

        /// @see file_printer::print_to_stream(std::ostream &s)
        void print_to_stream(std::ostream &s);
    };
}
