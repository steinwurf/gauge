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

        /// @copydoc format::print(std::ostream&, const std::vector&) const
        template<class T, class Alloc>
        void print(std::ostream &s, const std::vector<T,Alloc> &val) const
        {
            auto it = val.begin();

            s << "[";


            while(val.size() > 0)
            {
                print(s, *it);

                ++it;

                if(it == val.end())
                {
                    break;
                }
                else
                {
                    s << ",";
                }
            }

            s << "]";

        }


    };

}
