#pragma once

#include <memory>
#include <map>
#include <vector>
#include <cstdint>
#include <iostream>

#include <boost/any.hpp>

namespace gauge
{

    struct default_any_print
    {
    public:

        virtual void print(std::ostream &s, bool val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, short val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, unsigned short val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, int val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, unsigned int val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, long val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, unsigned long val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, float val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, double val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, long double val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, const void* val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, const std::string &val) const
        {
            s << val;
        }

        virtual void print(std::ostream &s, const boost::any &val) const
        {
            if(typeid(bool) == val.type())
                return print(s, boost::any_cast<bool>(val));
        }


    };




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

        /// Construct a new table
        table();

        /// Adds a new row to the table, a "row" measures the occurrence
        /// of a specific event or measurement type e.g. time elapsed.
        /// @param row The identifier for the new row.

        void add_column(const std::string& column);

    //     /// Called when new results are ready to be registered. This
    //     /// function essentially adds a new column to the table for all
    //     /// current rows, with zero initialized values.
    //     /// @param iterations The number of iterations performed for this
    //     ///        run.
        void add_row();

        template<class T>
        void set_value(const std::string& column, const T& value)
        {
            set_value(column, boost::any(value));
        }
        void set_value(const std::string& column, const boost::any& value);

    //     /// Updates the result for a specific row
    //     /// @param row The identifier for the row
    //     double& operator[](const std::string &row);

    //     /// @return The number of runs performed
    //     uint32_t runs() const;

    //     /// Set the unit of the results store in the table
    //     /// @param unit The unit used.
    //     void set_unit(const std::string &unit);

    //     /// @return The unit of the results stored in the table
    //     const std::string& unit() const;

    //     /// Print the table to the output stream
    //     /// @param o The output stream to which the table should be printed.
        void print(std::ostream& o, const default_any_print& fmt = default_any_print());

    //     /// @return The vector tracking the iterations per run
    //     const std::vector<uint64_t>& iterations() const;

    //     /// @return The vector containing the results for a specific row
    //     const std::vector<double>& row(const std::string &row) const;


    // public: // Iterator access to the results

    //     typedef std::map<std::string, std::vector<double> >::const_iterator
    //         const_iterator;

    //     /// @return const iterator to the first row
    //     const_iterator begin() const;

    //     /// @return const iterator to the last row
    //     const_iterator end() const;

    // private:

    //     /// The unit of the measurements
    //     std::string m_unit;

    //     /// Keeps track of the number of columns
        uint32_t m_rows;

    //     /// Keeps track of the number of iterations per run
    //     std::vector<uint64_t> m_iterations;

        /// The results per row
        std::map<std::string, std::vector<boost::any> > m_columns;

        struct column_info
        {
            bool m_updated;
            //std::type_info::hash_code m_hash_code;
            // boost::any m_fill;
        };

        /// Keeps track of which rows have been updated, this
        /// it to prevent multiple writers overwriting each other
        /// by accident
        std::map<std::string, column_info> m_columns_info;

    };

}
