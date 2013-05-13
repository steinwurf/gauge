#include "pylist.hpp"

namespace gauge
{

    pylist::pylist()
        : m_first(true)
    {}

    std::string pylist::str() const
    {
        std::string list = "[" + m_out.str() + "]";
        return list;
    }

    void pylist::clear()
    {
        m_first = true;
        m_out.str("");
    }

}
