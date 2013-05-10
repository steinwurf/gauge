#pragma once

#include <memory>
#include <map>
#include <vector>
#include <cstdint>
#include <iostream>
#include <cassert>

#include <boost/any.hpp>
#include <boost/optional.hpp>

#include "format.hpp"

namespace gauge
{
    /// The table class represents a set of measurements on for every
    /// run. The table consists of rows, the rows represent an event
    /// we have measured. In many cases we know the rows in advance,
    /// e.g. when using the time benchmark we have a row called elapsed
    /// which contains the elapsed time. However, in some cases we are
    /// running benchmarks where the "events" we measure are not known in
    /// advance. In these cases the table allows us to dynamically add rows
    /// as they are needed.
    /// The table will always preserve the runs <-> row mapping, this
    /// means that all rows will contain a result for any given run. If
    /// a row is added at a late stage the table will zero initialize the
    /// row for all previous runs. The same will happen if a row is not
    /// updated for a specific run.
    ///
    /// The table only is meant to store results with the same unit e.g.
    /// microseconds or ticks. If the unit of some event differ they should
    /// be stored in tow different tables.
    class table
    {
    public:

        /// Represents a column in the table
        struct column
        {
            std::vector<boost::any> m_values;
            bool m_updated;
            boost::optional<size_t> m_type_hash;
            boost::any m_fill;
        };

    public:

        /// Construct a new table
        table();

        /// Adds a new row to the table, a "row" measures the occurrence
        /// of a specific event or measurement type e.g. time elapsed.
        /// @param row The identifier for the new row.

        void add_column(const std::string& column);

        void set_column_type(const std::string& column,
                             const std::type_info& type_info);

        template<class T>
        void set_column_fill(const std::string& column,
                             const T& fill)
        {
            set_column_fill(column, boost::any(fill));
        }

        void set_column_fill(const std::string& column,
                             const boost::any& fill);

        /// Called when new results are ready to be registered. This
        /// function essentially adds a new column to the table for all
        /// current rows, with zero initialized values.
        /// @param iterations The number of iterations performed for this
        ///        run.
        void add_row();

        template<class T>
        void set_value(const std::string& column, const T& value)
        {
            set_value(column, boost::any(value));
        }

        void set_value(const std::string& column, const boost::any& value);

        void set_value(const std::string& column, const char* value)
        {
            set_value(column, std::string(value));
        }

        void merge(const table& src)
        {
            for(uint32_t i = 0; i < src.rows(); ++i)
            {
                add_row();

                for(const auto& t : src)
                {
                    set_value(t.first, t.second.m_values[i]);
                }

            }
        }

        /// @return The number of rows
        uint32_t rows() const;

        /// Print the table to the output stream
        /// @param o The output stream to which the table should be printed.
        void print(std::ostream& o, const format& fmt = format()) const;

        template<class T>
        bool is_column(const std::string &column) const
        {
            return is_column(column, typeid(T));
        }

        bool is_column(const std::string &column,
                       const std::type_info& type) const;

        /// @return The vector containing the results for a specific column
        template<class T>
        std::vector<T> column_as(const std::string &column) const
        {
            assert(m_columns.find(column) != m_columns.end());

            assert(is_column<T>(column));

            std::vector<T> v;

            const auto& c = m_columns.at(column);

            for(const auto& i : c.m_values)
            {
                assert(!i.empty());
                T t = boost::any_cast<T>(i);
                v.push_back(t);
            }

            return v;

        }

    public: // Iterator access to the results

        typedef std::map<std::string, column>::const_iterator
            const_iterator;

        /// @return const iterator to the first row
        const_iterator begin() const;

        /// @return const iterator to the last row
        const_iterator end() const;

    private:

        /// Keeps track of the number of columns
        uint32_t m_rows;

        /// Keeps track of which rows have been updated, this
        /// it to prevent multiple writers overwriting each other
        /// by accident
        std::map<std::string, column> m_columns;

    };

}
