#include <cstdint>

#include <gtest/gtest.h>

#include <gauge/result.hpp>

namespace gauge
{
    class writable
    {
    public:

    template<class T>
    writable(const T& t)
        : m_impl(new impl_t<T>(t))
        { }

        struct impl
        {
            virtual void write_(output &o) = 0;
        };

        template<class T>
        struct impl_t : public impl
        {
            impl_t(const T& t)
                : m_t(t)
            { }

            void write_(output &o)
            {
                write(o, m_t);
            }

        private:

            T m_t;

        };

        void write(output &o)
        {
            m_impl->write_(o);
        }

    std::unique_ptr<impl> m_impl;

    };
}


TEST(TestOutput, test_output)
{


    gauge::stream_output so(std::cout);
    gauge::output o(so);

    write(o, 10.5);

    std::cout << std::endl;

    gauge::measurement_data<uint32_t> v1;

    v1.add_result(10U, 10U);
    v1.add_result(100U, 10U);


    gauge::measurement_data<double> v2;
    gauge::measurement m1(std::move(v1));
    gauge::measurement m2(v2);

    std::vector<gauge::measurement> v;
    v.push_back(std::move(m1));
    v.push_back(std::move(m2));


    write(o, gauge::mean(v[0]));
    //write(o, v[0]);

    std::vector<gauge::measurement> c(std::move(v));

}



