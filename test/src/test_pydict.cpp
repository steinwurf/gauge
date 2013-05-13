#include <cstdint>

#include <gtest/gtest.h>

#include <gauge/pydict.hpp>

TEST(TestPyDict, test_pydict)
{
    gauge::pydict dict;

    uint32_t v1 = 10;
    std::vector<std::string> v2;
    v2.push_back("h");
    v2.push_back("he");
    v2.push_back("hej");

    dict.add("v1", v1);
    dict.add("v2", v2);

    EXPECT_EQ(dict.str(), "{'v1':10,'v2':['h','he','hej']}");
}


