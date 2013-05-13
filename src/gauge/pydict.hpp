#pragma once

#include <sstream>
#include <cassert>
#include <fstream>

#include "printer.hpp"
#include "benchmark.hpp"
#include "config_set.hpp"
#include "python_format.hpp"

namespace gauge
{

    /// The pydict object represents a python dict of
    /// objects. When a pydict is printed its content
    /// is encapsulated in {...} brackets.
    ///
    /// For example:
    ///
    ///     uint32_t v1 = 10;
    ///     std::vector<std::string> v2 = {"h","he","hej"};
    ///
    ///     pydict dict;
    ///     dict.add("v1", v1);
    ///     dict.add("v2", v2);
    ///
    ///     pyprint(std::cout, dict);
    ///
    /// Will result in the following output:
    ///
    ///     {"v1": 10, "v2": ["h", "he", "hej"]}
    struct pydict
    {
        /// Creates a new pydict
        pydict();

        /// Add a key and value to the dict
        /// @param key the dict entry key
        /// @param value the dict entry value
        template<class Key, class T>
        void add(const Key &key, const T &value)
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
                f.print(m_out, key);
                m_out << ":";
                f.print(m_out, value);
            }

        /// Resets the state of the pydict
        void clear();

        /// @return the dict content as a string
        std::string str() const;

    private:

        /// Keeps track of whether an entry is the first
        /// in the dictionary
        bool m_first;

        /// Buffer for the added content
        std::stringstream m_out;
    };

}



