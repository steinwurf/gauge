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
    /// run. The table consists of columns and rows, the columns represent
    /// an event we have measured, the rows the values.
    /// In many cases we know the columns in advance,
    /// e.g. when using the time benchmark we have a column called time
    /// which contains the elapsed time. However, in some cases we are
    /// running benchmarks where the "events" we measure are not known in
    /// advance. In these cases the table allows us to dynamically add
    /// columns as they are needed.
    /// The table will always preserve the runs <-> row mapping, this
    /// means that all rows will contain a result for any given run. If
    /// a row is added at a late stage the table will use the fill value
    /// to initialize the row for all previous runs. The same will happen
    /// if a row is not updated for a specific run.
    ///
    /// The table only is meant to store results with the same unit e.g.
    /// microseconds or ticks. If the unit of some event differ they should
    /// be stored in two different tables.
    class table
    {
    public:

        /// Represents a column in the table
        struct column
        {
            /// Stores all the values for the table rows
            std::vector<boost::any> m_values;

            /// Keeps track of whether a row has already been
            /// initialized
            bool m_updated;

            /// Stores the data type of the row can be obtained using
            /// typeid(int).hash_code()
            boost::optional<size_t> m_type_hash;

            /// The fill value for the column
            boost::any m_fill;
        };

    public:

        /// Construct a new table
        table();

        /// Create a new column in the table
        /// @param column The name of the column
        /// @param fill The value used to fill new rows
        void add_column(const std::string& column,
                        const boost::any& fill = boost::any());

        /// Set the column data type of the column
        /// @param column The name of the column
        /// @param type_info The type info of the column data type
        void set_column_type(const std::string& column,
                             const std::type_info& type_info);

        /// Set/change the fill value for a column
        /// @param column The name of the column
        /// @param fill The fill value
        template<class T>
        void set_column_fill(const std::string& column,
                             const T& fill)
        {
            set_column_fill(column, boost::any(fill));
        }

        /// Set/change the fill value for a column
        /// @param column The name of the column
        /// @param fill The fill value
        void set_column_fill(const std::string& column,
                             const boost::any& fill);

        /// Called when new results are ready to be registered. This
        /// function essentially adds a new row to the table for all
        /// current columns.
        void add_row();

        /// Sets the value for the current row in the specified column
        /// @param column The name of the column
        /// @param value The value to set for the current row
        template<class T>
        void set_value(const std::string& column, const T& value)
        {
            set_value(column, boost::any(value));
        }

        /// Sets the value for the current row in the specified column
        /// @param column The name of the column
        /// @param value The value to set for the current row
        void set_value(const std::string& column, const boost::any& value);

        /// Sets the value for the current row in the specified column
        /// @param column The name of the column
        /// @param value The value to set for the current row
        void set_value(const std::string& column, const char* value)
        {
            set_value(column, std::string(value));
        }

        /// Merge the source table into this table. All rows in the source
        /// table are added as new rows in the table and columns are
        /// created on-the-fly as needed.
        /// @param src The source table to merge into this table
        void merge(const table& src);

        /// @return The number of rows
        uint32_t rows() const;

        /// Print the table to the output stream
        /// @param o The output stream to which the table should be printed.
        void print(std::ostream& o, const format& fmt = format(),
                   const std::string& seperator = ",") const;

        /// Checks whether the column has a specific data type
        /// @param column The name of the column
        /// @return True if the column has the type T
        template<class T>
        bool is_column(const std::string &column) const
        {
            return is_column(column, typeid(T));
        }

        /// Checks whether the column has a specific data type
        /// @param column The name of the column
        /// @param type The data type of the column
        /// @return True if the column has the type
        bool is_column(const std::string &column,
                       const std::type_info& type) const;

        /// Returns true if the specific column exists
        /// @param column The name of the column
        /// @return True if the column exists
        bool has_column(const std::string& column) const;

        /// Returns a specific column.
        /// @param column The name of the column
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

        /// The iterator type
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
