#include <cstdint>

#include <gtest/gtest.h>

#include <gauge/result.hpp>





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

    std::vector<gauge::measurement> c(std::move(v));

}



