/// Modified from Nick Bruun's Hayai C++ Benchmark library
/// See LICENSE.rst
#ifndef GAUGE_CONSOLE_COLORS_UNIX_H
#define GAUGE_CONSOLE_COLORS_UNIX_H

#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "console_colors.h"

namespace gauge
{

    /// Makes it possible to disable terminal colors
    /// by running:
    ///
    ///    export TERM=dumb
    ///
    /// @return true if the terminal supports colors
    inline bool has_colors()
    {
        if (::isatty(STDOUT_FILENO))
        {
            char *term = ::getenv("TERM");

            if (term && ::strcmp(term, "dumb"))
                return true;
        }
        return false;
    }

    /// Static helper class for outputting to a terminal/console.
    class console_impl
    {
    public:

        static int color_code(const console::textcolor& color)
            {
                int c = 0;

                switch(color)
                {
                case console::textblack: c = 30; break;
                case console::textblue: c = 34; break;
                case console::textgreen: c = 32; break;
                case console::textcyan: c = 36; break;
                case console::textred: c = 31; break;
                case console::textpurple: c = 35; break;
                case console::textyellow: c = 33; break;
                case console::textwhite: c = 37; break;
                default: assert(0);
                }

                return c;
            }

        static void print_color(std::ostream &stream,
                                const console::textcolor& color)
            {
                if(!has_colors())
                    return;

                if (color == gauge::console::textdefault)
                    stream << "\033[m";
                else
                    stream << "\033[0;" << color_code(color) << "m";
            }
    };
}


#endif


