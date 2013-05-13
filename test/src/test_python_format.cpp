#include <cstdint>

#include <gtest/gtest.h>

#include <gauge/python_format.hpp>

TEST(TestPythonFormat, test_python_format)
{
    std::stringstream ss;
    gauge::python_format fmt;

    fmt.print(ss, bool(true));
    fmt.print(ss, int8_t(-1));
    fmt.print(ss, uint8_t(1));
    fmt.print(ss, int16_t(-1));
    fmt.print(ss, uint16_t(1));
    fmt.print(ss, int32_t(-1));
    fmt.print(ss, uint32_t(1));
    fmt.print(ss, int64_t(-1));
    fmt.print(ss, uint64_t(1));

    fmt.print(ss, float(-3.14));
    fmt.print(ss, double(3.14));

    fmt.print(ss, "test");
    fmt.print(ss, std::string("test"));

    std::vector<int8_t> v;
    v.push_back(-1);
    v.push_back(1);

    fmt.print(ss, v);

    EXPECT_EQ(ss.str(), "True-11-11-11-11-3.143.14'test''test'[-1,1]");
}


