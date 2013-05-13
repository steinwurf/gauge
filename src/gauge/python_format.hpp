#pragma once

#include <sstream>

#include "format.hpp"

namespace gauge
{

    struct pylist;
    struct pydict;

    /// Prints to the ostrea in Python format
    struct python_format : public format
    {
    public:

        /// Make the format::print functions available
        using format::print;

    public:

        void print(std::ostream &s, const pylist &val) const;

        void print(std::ostream &s, const pydict &val) const;

        /// @copydoc format::print(std::ostream&,bool) const
        void print(std::ostream &s, bool val) const;

        /// @copydoc format::print(std::ostream&, const std::string&) const
        void print(std::ostream &s, const std::string &val) const;

        std::string vector_begin() const
        { return "["; }

        std::string vector_end() const
        { return "]"; }

    };

}

