// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>

namespace gauge
{
    /// Returns a string representing the Steinwurf version of this library.
    inline std::string steinwurf_version()
    {
        #ifdef STEINWURF_GAUGE_VERSION
        return STEINWURF_GAUGE_VERSION;
        #else
        return "unknown";
        #endif
    }
}
