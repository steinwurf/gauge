// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gtest/gtest.h>

#include <cstdint>
#include <ctime>

#include <gauge/gauge.hpp>

TEST(test_gauge, run_benchmarks)
{
    // Dummy arguments for gauge::runner
    int argc = 1;
    const char* argv[] = { "program" };

    gauge::runner::add_default_printers();
    gauge::runner::run_benchmarks(argc, argv);
}

GTEST_API_ int main(int argc, char** argv)
{
    srand(static_cast<uint32_t>(time(0)));

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
