#pragma once

#include <sstream>

#include "python_format.hpp"

namespace gauge
{

    /// The pylist object represents a python list of
    /// objects. When a pylist is printed its content
    /// is encapsulated in [...] brackets.
    ///
    /// For example:
    ///
    ///     uint32_t v1 = 10;
    ///     std::vector<std::string> v2 = {"h","he","hej"};
    ///
    ///     pylist list;
    ///     list.add(v1);
    ///     list.add(v2);
    ///
    ///     pyprint(std::cout, list);
    ///
    /// Will result in the following output:
    ///
    ///     [10, ["h", "he", "hej"]]
    struct pylist
    {
        /// Create a new pylist object
        pylist();

        /// Add a value to the pylist
        /// @param value the value to be printed to the pylist
        template<class T>
        void add(const T &value)
            {
                if(m_first)
                {
                    m_first = false;
                }
                else
                {
                    m_out << ",";
                }

                python_format f;
                f.print(m_out, value);
            }

        /// @return the content of the list as a string
        std::string str() const;

        /// Clears the pylist content so that a new list
        /// may be created
        void clear();

    private:

        /// Keeps track of which element is the newest
        bool m_first;

        /// Buffer of the already added values
        std::stringstream m_out;
    };

}



