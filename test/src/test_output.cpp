#include <cstdint>

#include <gtest/gtest.h>

#include <gauge/result.hpp>

TEST(TestOutput, test_output)
{


    gauge::stream_output so(std::cout);
    gauge::output o(so);

    format(o, 10.5);

    gauge::measurement_data<uint32_t> v;
    gauge::measurement m(v);

    mean(m, o);

}



