/// Modified from Nick Bruun's Hayai C++ Benchmark library
/// See LICENSE.rst

#pragma once

#include <iostream>

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
            textdefault,

            /// Black.
            /// @warning Avoid using black unless it is absolutely necessary.
            textblack,

            /// Blue.
            textblue,

            /// Green.
            textgreen,

            /// cyan.
            textcyan,

            /// red.
            textred,

            /// purple.
            textpurple,

            /// Yellow.
            textyellow,

            /// White.
            /// @warning Avoid using white unless it is absolutely necessary.
            textwhite
        };
    };

    /// Output operator for writing a color to the terminal. The function
    /// will try to determine whether any terminal color support exists
    /// @param stream the output stream
    /// @param color the selected color
    std::ostream& operator<<(std::ostream& stream,
                             const gauge::console::textcolor& color);
}
