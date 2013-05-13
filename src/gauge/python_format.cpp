#include "python_format.hpp"

#include "pylist.hpp"
#include "pydict.hpp"

namespace gauge
{

    void python_format::print(std::ostream &s, const pylist &val) const
    {
        s << val.str();
    }

    void python_format::print(std::ostream &s, const pydict &val) const
    {
        s << val.str();
    }

    void python_format::print(std::ostream &s, bool val) const
    {
        if(val)
        {
            s << "True";
        }
        else
        {
            s << "False";
        }
    }

    void python_format::print(std::ostream &s, const std::string &val) const
    {
        s << "'" << val << "'";
    }

}
