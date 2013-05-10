#include "table.hpp"

#include <cassert>


namespace gauge
{

    table::table()
        : m_rows(0)
    { }

    void table::add_column(const std::string &column)
    {
        assert(m_columns.find(column) == m_columns.end());

        auto& c = m_columns[column];
        c.m_values.resize(m_rows);
        c.m_updated = false;
    }

    void table::set_column_type(const std::string& column,
                                const std::type_info& type_info)
    {
        assert(m_columns.find(column) != m_columns.end());

        auto& c = m_columns[column];

        assert(!c.m_type_hash);
        c.m_type_hash = type_info.hash_code();
    }

    void table::set_column_fill(const std::string& column,
                                const boost::any& value)
    {
        assert(!value.empty());

        assert(m_columns.find(column) != m_columns.end());

        auto& c = m_columns[column];

        if(!c.m_type_hash)
        {
            c.m_type_hash = value.type().hash_code();
        }

        assert(c.m_type_hash == value.type().hash_code());

        c.m_fill = value;
    }

    bool table::is_column(const std::string& column,
                          const std::type_info& type) const
    {
        assert(m_columns.find(column) != m_columns.end());
        const auto& c = m_columns.at(column);
        assert(c.m_type_hash);
        return (*c.m_type_hash) == type.hash_code();

    }

    void table::add_row()
    {
        ++m_rows;

        for(auto& c: m_columns)
        {
            c.second.m_updated = false;
            c.second.m_values.resize(m_rows, c.second.m_fill);
        }
    }

    void table::set_value(const std::string& column, const boost::any& value)
    {
        assert(!value.empty());

        if(m_columns.find(column) == m_columns.end())
        {
            add_column(column);
        }

        assert(m_columns.find(column) != m_columns.end());

        auto& c = m_columns[column];

        if(!c.m_type_hash)
        {
            set_column_type(column, value.type());
        }

        assert(c.m_updated == false);
        assert(c.m_type_hash);
        assert((*c.m_type_hash) == value.type().hash_code());

        c.m_updated = true;

        assert(m_rows > 0); // Did you forget to call add_row(..)
        assert(c.m_values.size() == m_rows);

        // Access the "current" row if we are on row 1,
        // this is index 0
        c.m_values[m_rows - 1] = value;
    }

    uint32_t table::rows() const
    {
        return m_rows;
    }

    void table::print(std::ostream& o, const format& fmt)
    {

        for(auto& c: m_columns)
        {
            o << "  " << c.first << ": ";
            for(const auto& v: c.second.m_values)
            {
                fmt.print(o, v);
                o << " ";
            }
            o << std::endl;
        }
    }


    // const std::vector<uint64_t>& table::iterations() const
    // {
    //     return m_iterations;
    // }

    // table::const_iterator table::begin() const
    // {
    //     return m_results.cbegin();
    // }

    // table::const_iterator table::end() const
    // {
    //     return m_results.cend();
    // }


}
