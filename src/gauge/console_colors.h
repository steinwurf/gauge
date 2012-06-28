/// Modified from Nick Bruun's Hayai C++ Benchmark library
/// See LICENSE.rst
#ifndef GAUGE_THIRD_PARTY_CONSOLE_COLORS_CONSOLE_COLORS_H
#define GAUGE_THIRD_PARTY_CONSOLE_COLORS_CONSOLE_COLORS_H

namespace gauge
{

    /// Static helper class for outputting to a terminal/console.
    class console
    {
    public:

        /// Console text colors.
        enum textcolor
        {
            /// Default console color.
            /// Used for resets.
            textdefault = 0,

            /// Black.
            /// @warning Avoid using black unless it is absolutely necessary.
            textblack = 30,

            /// Blue.
            textblue = 34,

            /// Green.
            textgreen = 32,

            /// cyan.
            textcyan = 36,

            /// red.
            textred = 31,

            /// purple.
            textpurple = 35,

            /// Yellow.
            textyellow = 33,

            /// White.
            /// @warning Avoid using white unless it is absolutely necessary.
            textwhite = 37
        };
    };
}

inline std::ostream& operator <<(std::ostream& stream,
                                 const gauge::console::textcolor& color)
{
    if (color == gauge::console::textdefault)
        return stream << "\033[m";
    else
        return stream << "\033[0;" << int(color) << "m";
}

#endif


