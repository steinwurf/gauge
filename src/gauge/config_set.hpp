#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <cstdint>
#include <string>
#include <boost/any.hpp>
#include <tables/format.hpp>

namespace gauge
{

    struct config_set
    {

        typedef std::map<std::string, boost::any> config_map;
        typedef config_map::const_iterator const_iterator;

        template<class T>
        void set_value(const std::string &key, T v)
        {
            m_values[key] = boost::any(v);
        }

        template<class T>
        T get_value(const std::string &key) const
        {
            assert(m_values.find(key) != m_values.end());
            assert(typeid(T) == m_values.at(key).type());

            T value = boost::any_cast<T>(m_values.at(key));
            return value;
        }

        const_iterator begin() const
        { return m_values.begin(); }

        const_iterator end() const
        { return m_values.end(); }

        uint64_t size() const
        {
            return m_values.size();
        }

        void print(std::ostream &os) const
        {
            auto it = begin();

            tables::format f;

            if(it != end())
            {
                os << (it->first) << "=";
                f.print(os, it->second);
            }
            ++it;

            while(it != end())
            {
                os << ", " << (it->first) << "=";
                f.print(os, it->second);
                ++it;
            }
        }

        config_map m_values;
    };


    inline std::ostream& operator<<(std::ostream &os, const config_set &cs)
    {
        cs.print(os);
        return os;
    }


}



