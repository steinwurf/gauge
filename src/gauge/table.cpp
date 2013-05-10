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
        assert(m_columns_info.find(column) == m_columns_info.end());

        m_columns[column].resize(m_rows);
        auto& info = m_columns_info[column];

        info.m_updated = false;
    }

    void table::set_column_type(const std::string& column,
                                const std::type_info& type_info)
    {
        assert(m_columns.find(column) != m_columns.end());
        assert(m_columns_info.find(column) != m_columns_info.end());

        auto& info = m_columns_info[column];

        assert(!info.m_type_hash);
        info.m_type_hash = type_info.hash_code();
    }

    void table::set_column_fill(const std::string& column,
                                const boost::any& value)
    {
        assert(!value.empty());

        if(m_columns.find(column) == m_columns.end())
        {
            add_column(column);
        }

        assert(m_columns.find(column) != m_columns.end());
        assert(m_columns_info.find(column) != m_columns_info.end());

        auto& info = m_columns_info[column];

        if(!info.m_type_hash)
        {
            info.m_type_hash = value.type().hash_code();
        }

        assert(info.m_type_hash == value.type().hash_code());

        info.m_fill = value;
    }

    bool table::is_column(const std::string& column,
                          const std::type_info& type) const
    {
        assert(m_columns_info.find(column) != m_columns_info.end());
        auto& info = m_columns_info.at(column);
        assert(info.m_type_hash);
        return *info.m_type_hash == type.hash_code();

    }

    void table::add_row()
    {
        ++m_rows;

        for(auto& v: m_columns)
        {
            assert(m_columns_info.find(v.first) != m_columns_info.end());

            auto& info = m_columns_info[v.first];
            info.m_updated = false;
            v.second.resize(m_rows, info.m_fill);
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
        assert(m_columns_info.find(column) != m_columns_info.end());

        auto& info = m_columns_info[column];

        if(!info.m_type_hash)
        {
            set_column_type(column, value.type());
        }

        assert(info.m_updated == false);
        assert(info.m_type_hash);
        assert(*info.m_type_hash == value.type().hash_code());

        info.m_updated = true;

        assert(m_rows > 0); // Did you forget to call add_row(..)
        auto& v = m_columns[column];
        assert(v.size() == m_rows);

        // Access the "current" row if we are on row 1,
        // this is index 0
        v[m_rows - 1] = value;
    }

    // uint32_t table::runs() const
    // {
    //     return m_runs;
    // }

    // void table::set_unit(const std::string &unit)
    // {
    //     m_unit = unit;
    // }

    // const std::string& table::unit() const
    // {
    //     assert(!m_unit.empty());
    //     return m_unit;
    // }

    void table::print(std::ostream& o, const format& fmt)
    {

        for(auto& v: m_columns)
        {
            o << v.first << "\t";
        }
        o << std::endl;

        for(auto& v: m_columns)
        {
            o << "  " << v.first << ": ";
            for(const auto& r: v.second)
            {
                fmt.print(o, r);
                o << " ";
            }
            o << std::endl;
        }
    }

    // const std::vector<double>& table::row(const std::string &row) const
    // {
    //     assert(m_results.find(row) != m_results.end());
    //     return m_results.at(row);
    // }

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
