#include "table.hpp"

#include <cassert>


namespace gauge
{

    table::table() :
        m_runs(0)
    { }

    void table::add_row(const std::string &row)
    {
        assert(m_results.find(row) == m_results.end());
        assert(m_updated.find(row) == m_updated.end());

        m_results[row].resize(m_runs, 0);
        m_updated[row] = false;
    }

    void table::add_run(uint64_t iterations)
    {
        ++m_runs;
        m_iterations.push_back(iterations);
        assert(m_runs == m_iterations.size());

        for(auto& v: m_results)
        {
            v.second.resize(m_runs, 0);
        }

        for(auto& v: m_updated)
        {
            v.second = false;
        }
    }

    double& table::operator[](const std::string &row)
    {
        if(m_results.find(row) == m_results.end())
            add_row(row);

        assert(m_results.find(row) != m_results.end());
        assert(m_updated.find(row) != m_updated.end());

        assert(m_updated[row] == false);
        m_updated[row] = true;

        assert(m_runs > 0); // Did you forget to call add_row(..)
        auto& v = m_results[row];
        assert(v.size() == m_runs);

        // Access the "current" run if we are on run 1,
        // this is index 0
        return v[m_runs - 1];
    }

    uint32_t table::runs() const
    {
        return m_runs;
    }

    void table::set_unit(const std::string &unit)
    {
        m_unit = unit;
    }

    const std::string& table::unit() const
    {
        assert(!m_unit.empty());
        return m_unit;
    }

    void table::print(std::ostream& o)
    {

        o << "unit: " << m_unit << std::endl;

        o << "  " << "iterations: ";
        for(auto& i: m_iterations)
        {
            o << i << " ";
        }
        o << std::endl;

        for(auto& v: m_results)
        {
            o << "  " << v.first << ": ";
            for(auto& r: v.second)
            {
                o << r << " ";
            }
            o << std::endl;
        }
    }

    const std::vector<double>& table::row(const std::string &row) const
    {
        assert(m_results.find(row) != m_results.end());
        return m_results.at(row);
    }

    const std::vector<uint64_t>& table::iterations() const
    {
        return m_iterations;
    }

    table::const_iterator table::begin() const
    {
        return m_results.cbegin();
    }

    table::const_iterator table::end() const
    {
        return m_results.cend();
    }


}
