// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "console_colors.hpp"

// The preprocessor defines are taken from
// http://predef.sourceforge.net
#if defined(__unix__)
  #include "console_colors_unix.hpp"
#else
  #include "console_colors_none.hpp"
#endif

// @todo implement colors for windows
//#elif defined(_WIN32)
//  #include "console_colors_win32.h"


namespace gauge
{
    std::ostream& operator<<(std::ostream& stream,
                              const console::textcolor& color)
    {
        console_impl::print_color(stream, color);
        return stream;
    }
}
