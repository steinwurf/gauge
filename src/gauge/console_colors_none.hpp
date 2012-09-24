/// Modified from Nick Bruun's Hayai C++ Benchmark library
/// See LICENSE.rst

#ifndef GAUGE_CONSOLE_COLORS_NONE_HPP
#define GAUGE_CONSOLE_COLORS_NONE_HPP

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

#endif

