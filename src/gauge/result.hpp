#pragma once

#include <memory>

namespace gauge
{

    ///
    class output
    {
    public:


        template<class T>
        output(const T& t)
            : m_impl(new impl_t<T>(t))
        { }

        struct impl
        {
            virtual void format_(uint64_t) = 0;
            virtual void format_(uint32_t) = 0;
            virtual void format_(double) = 0;
        };

        template<class T>
        struct impl_t : public impl
        {
            impl_t(const T& t)
                : m_t(t)
            { }

            void format_(uint32_t v)
            {
                format(m_t, v);
            }

            void format_(uint64_t v)
            {
                format(m_t, v);
            }

            void format_(double v)
            {
                format(m_t, v);
            }


        private:

            T m_t;

        };

        template<class U>
        friend void format(output &o, U v)
        {
            o.m_impl->format_(v);
        }

        std::unique_ptr<impl> m_impl;

    };


    /// Struct containing the benchmark results from a single
    /// run.
    // template<class T>
    // struct measurement
    // {

    //     /// The result per iteration
    //     T m_result;

    //     /// The number of iterations performed to obtain the
    //     /// corresponding result
    //     uint64_t m_iterations;

    // };

    template<class T>
    struct measurement_data
    {

        /// The name of this result - describes the result type e.g.
        /// "time" or "throughput"
        std::string m_name;

        /// The unit of this result e.g. seconds or MB/s
        std::string m_unit;

        /// The collection of measurements
        std::vector<T> m_measurements;

        /// The number of iterations per measurement
        std::vector<uint64_t> m_iterations;

    };

    template<class T>
    void mean(measurement_data<T> &m, output &out)
    {
        format(out, m.m_measurements.size());
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
            virtual void mean_(output&) = 0;
        };

        template<class T>
        struct impl_t : public impl
        {
            impl_t(const T& t)
                : m_t(t)
            { }

            void mean_(output &o)
            {
                mean(m_t, o);
            }

        private:

            T m_t;

        };

        friend void mean(measurement &m, output &o)
        {
            m.m_impl->mean_(o);
        }

        std::unique_ptr<impl> m_impl;

    };


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
        friend void format(stream_output &, T);

    private:

        std::ostream &m_out;
    };

    template<class T>
    void format(stream_output &s, T v)
    {
        s.m_out << v;
    }


}
