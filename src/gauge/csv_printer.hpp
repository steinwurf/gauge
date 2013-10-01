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
        /// without the file extension
        csv_printer(const std::string& default_filename = "out");

    public: // From printer

        /// @see printer::set_options(po::variables_map&);
        void set_options(po::variables_map& options);

    public: // From file_printer

        /// @see file_printer::print_to_stream(std::ostream &s)
        void print_to_stream(std::ostream &s);

    private:

        /// Store the value separator
        std::string m_value_seperator;
    };
}
