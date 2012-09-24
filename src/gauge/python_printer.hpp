#ifndef GAUGE_PYTHON_PRINTER_HPP
#define GAUGE_PYTHON_PRINTER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <fstream>

#include "printer.hpp"
#include "benchmark.hpp"
#include "config_set.hpp"

namespace gauge
{
    template<class T>
    inline void pyprint(std::ostream &os, const T &v)
    {
        os << v;
    }

    inline void pyprint(std::ostream &os, const std::string &v)
    {
        os << "\"" << v << "\"";
    }

    inline void pyprint(std::ostream &os, const char *v)
    {
        os << "\"" << v << "\"";
    }

    inline void pyprint(std::ostream &os, const config_type &v)
    {
        default_format f;
        v.print(os, f);
    }

    /// Prints a std::vector<T> as a python list
    /// @param os, the output stream
    /// @param v, the vector
    template<class T, class Alloc>
    inline void pyprint(std::ostream &os, const std::vector<T, Alloc> &v)
    {
        auto it = v.begin();

        os << "[";

        while(v.size() > 0)
        {
            pyprint(os, *it);

            ++it;

            if(it == v.end())
            {
                break;
            }
            else
            {
                os << ",";
            }
        }

        os << "]";
    }


    struct python_format : public default_format
    {
        virtual void format(std::ostream &s, const std::string &val) const
            {
                s << "\"" << val << "\"";
            }
    };

    inline void pyprint(std::ostream &os, const config_set &set)
    {
        auto it = set.begin();

        os << "{";

        while(set.size() > 0)
        {
            pyprint(os, it->first);
            os << ":";
            it->second->print(os, python_format());

            ++it;

            if(it == set.end())
            {
                break;
            }
            else
            {
                os << ",";
            }
        }

        os << "}";
    }

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

                pyprint(m_out, value);
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

    /// Prints a pylist to the ostream
    /// @param os the ostream to output to
    /// @param v the pylist to print
    inline void pyprint(std::ostream &os, const pylist &v)
    {
        os << v.str();
    }


    /// The pylist object represents a python dict of
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

                pyprint(m_out, key);
                m_out << ":";
                pyprint(m_out, value);
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

    /// Prints the pydict to the specified ostream
    /// @param os the ostream write to
    /// @param v the pydict object to write
    inline void pyprint(std::ostream &os, const pydict &v)
    {
        os << v.str();
    }

    /// A python result printer implementation. The purpose
    /// of this printer is to "dump" results to a .py file
    /// which then may be easily used in python scripts for
    /// plotting etc.
    class python_printer : public printer
    {
    public:

        /// Create a new python printer
        /// @param filename the filename to use e.g. out.py
        python_printer(const std::string &filename);

    public: // From printer

        /// @see printer::start_benchmark()
        void start_benchmark(/*const gauge_info &infouint32_t benchmarks*/);

        /// @see printer::benchmark_result()
        void benchmark_result(const benchmark &info, const result &result);

        /// @see printer::end_benchmark()
        void end_benchmark(/*const benchmark &info*/);

    private:

        /// Results are printed as a python list using this
        /// pylist
        pylist m_list;

        /// Store the filename of the output file
        std::string m_filename;

        /// The output file stream
        std::ofstream m_out;
    };

}

#endif

