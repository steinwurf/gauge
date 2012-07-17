#include "console_colors.h"

// The preprocessor defines are taken from
// http://predef.sourceforge.net
#if defined(__unix__)
  #include "console_colors_unix.h"
#else
  #include "console_colors_none.h"
#endif

// @todo implement colors for windows
//#elif defined(_WIN32)
//  #include "console_colors_win32.h"


namespace gauge
{
    std::ostream& operator <<(std::ostream& stream,
                              const console::textcolor& color)
    {
        console_impl::print_color(stream, color);
        return stream;
    }
}




