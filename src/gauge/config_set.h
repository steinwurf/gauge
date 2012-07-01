#ifndef GAUGE_CONFIG_SET_H
#define GAUGE_CONFIG_SET_H

#include <iostream>
#include <memory>
#include <map>
#include <cstdint>

namespace gauge
{

    struct default_format
    {
    public:

        virtual void format(std::ostream &s, bool val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, short val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, unsigned short val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, int val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, unsigned int val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, long val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, unsigned long val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, float val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, double val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, long double val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, const void* val) const
            {
                s << val;
            }

        virtual void format(std::ostream &s, const std::string &val) const
            {
                s << val;
            }


    };

    struct config_type
    {
        virtual void print(std::ostream &s,
                           const default_format &format = default_format()) const
            {
                do_print(s, format);
            }

        virtual void do_print(std::ostream &s, const default_format &format) const = 0;
    };


    template<class T>
    struct config_value : public config_type
    {
    public:

        void set(T value)
            { m_value = value; }

        T get() const
            { return m_value; }

        void do_print(std::ostream &s, const default_format &format) const
            { return format.format(s, m_value); }

        T m_value;
    };


    struct config_set
    {
        typedef std::shared_ptr<config_type> config_ptr;
        typedef std::map<std::string, config_ptr> config_map;
        typedef config_map::const_iterator const_iterator;

        template<class T>
        void set_value(const std::string &key, T v)
            {
                auto value = std::make_shared< config_value<T> >();
                value->set(v);

                m_values[key] = value;
            }

        template<class T>
        T get_value(const std::string &key)
            {
                auto value = std::dynamic_pointer_cast< config_value<T> >(
                    m_values[key]);

                assert(value);

                return value->get();
            }

        const_iterator begin() const
            { return m_values.begin(); }

        const_iterator end() const
            { return m_values.end(); }

        uint32_t size() const
            {
                return m_values.size();
            }

        void print(std::ostream &os) const
            {
                auto it = begin();

                default_format f;

                if(it != end())
                {
                    os << (it->first) << "=";
                    it->second->print(os,f);
                }
                ++it;

                while(it != end())
                {
                    os << ", " << (it->first) << "=";
                    it->second->print(os,f);
                    ++it;
                }
            }

        config_map m_values;
    };


}


inline std::ostream& operator<<(std::ostream &os, const gauge::config_set &cs)
{
    cs.print(os);
    return os;
}

#endif

