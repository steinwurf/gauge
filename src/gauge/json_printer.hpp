// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <ostream>
#include <string>

#include "file_printer.hpp"

namespace gauge
{
    /// A json result printer implementation. The purpose
    /// of this printer is to "dump" results to a .json file
    class json_printer : public file_printer
    {
    public:
        /// Create a new json printer
        /// @param default_filename The default name of the outputted file
        explicit json_printer(const std::string& default_filename = "out.json");

    public:
        // From file_printer
        /// @see file_printer::print_to_stream(std::ostream &s)
        void print_to_stream(std::ostream &s);
    };
}
