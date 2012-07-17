/// Modified from Nick Bruun's Hayai C++ Benchmark library
/// See LICENSE.rst
#ifndef GAUGE_CONSOLE_COLORS_NONE_H
#define GAUGE_CONSOLE_COLORS_NONE_H

#include "console_colors.h"

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


