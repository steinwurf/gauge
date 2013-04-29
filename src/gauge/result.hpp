#pragma once

#include <memory>

namespace gauge
{

    class output
    {
    public:


        template<class T>
        output(const T& t)
            : m_impl(new impl_t<T>(t))
        { }

        struct impl
        {
            virtual void write_(uint64_t) = 0;
            virtual void write_(uint32_t) = 0;
            virtual void write_(double) = 0;
        };

        template<class T>
        struct impl_t : public impl
        {
            impl_t(const T& t)
                : m_t(t)
            { }

            void write_(uint32_t v)
            {
                write(m_t, v);
            }

            void write_(uint64_t v)
            {
                write(m_t, v);
            }

            void write_(double v)
            {
                write(m_t, v);
            }


        private:

            T m_t;

        };

        template<class U>
        friend void write(output &o, U v)
        {
            o.m_impl->write_(v);
        }

        std::unique_ptr<impl> m_impl;

    };


    /// Struct containing the benchmark results from a single
    /// run.
    template<class T>
    struct result
    {

        /// Constructor
        result(const T &value, uint64_t iterations)
            : m_value(value), m_iterations(iterations)
        { }

        /// The result per iteration
        T m_value;

        /// The number of iterations performed to obtain the
        /// corresponding result
        uint64_t m_iterations;

    };

    template<class T>
    struct measurement_data
    {

        /// Add value
        void add_result(T value, uint64_t iterations)
        {
            m_measurements.push_back(result<T>(value, iterations));
        }

        /// The name of this result - describes the result type e.g.
        /// "time" or "throughput"
        std::string m_name;

        /// The unit of this result e.g. seconds or MB/s
        std::string m_unit;

        /// The collection of measurements
        std::vector< result<T> > m_measurements;

    };

    template<class T>
    void write_mean(output &out, measurement_data<T> &m)
    {
        uint32_t size = m.m_measurements.size();

        double sum = 0.0;
        for(auto& v : m.m_measurements)
            sum += (double) v.m_value;

        write(out, sum / size);
    }

    template<class T>
    void write_max(output &out, measurement_data<T> &m)
    {
        write(out, 100U);
    }


    class measurement
    {
    public:

        template<class T>
        measurement(const T& t)
            : m_impl(new impl_t<T>(t))
        { }

        struct impl
        {
            virtual void write_mean_(output&) = 0;
            virtual void write_max_(output&) = 0;
        };

        template<class T>
        struct impl_t : public impl
        {
            impl_t(const T& t)
                : m_t(t)
            { }

            void write_mean_(output &o)
            {
                write_mean(o, m_t);
            }

            void write_max_(output &o)
            {
                write_max(o, m_t);
            }


        private:

            T m_t;

        };

        friend void write_mean(output &o, measurement &m)
        {
            m.m_impl->write_mean_(o);
        }

        friend void write_max(output &o, measurement &m)
        {
            m.m_impl->write_max_(o);
        }


        std::unique_ptr<impl> m_impl;

    };

    struct mean
    {
        mean(measurement &m)
            : m_m(m)
        { }

        measurement &m_m;
    };

    struct max
    {
        max(measurement &m)
            : m_m(m)
        { }

        measurement &m_m;
    };


    void write(output &out, const mean &m)
    {
        write_mean(out, m.m_m);
    }

    void write(output &out, const max &m)
    {
        write_max(out, m.m_m);
    }


    // struct statisticts
    // {


    //     struct impl
    //     {
    //         virtual mean_(const output &out) = 0;
    //     };

    //     template<class T>
    //     struct impl_t
    //     {

    //         void mean_(const output &out)
    //         {
    //             mean(m_t, out);
    //         }


    //     private:

    //         T m_t;
    //     };



    // };




    struct stream_output
    {
    public:
        stream_output(std::ostream &out)
            : m_out(out)
        {}

        template<class T>
        friend void write(stream_output &, T);

    private:

        std::ostream &m_out;
    };

    template<class T>
    void write(stream_output &s, T v)
    {
        s.m_out << v;
    }


}
