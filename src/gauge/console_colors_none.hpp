/// Modified from Nick Bruun's Hayai C++ Benchmark library
/// See LICENSE.rst

#pragma once

#include "console_colors.hpp"

namespace gauge
{
    class console_impl
    {
    public:

        /// No color support so just return
        static void print_color(std::ostream &/*stream*/,
                                const console::textcolor &/*color*/)
            {
                return;
            }
    };
}
