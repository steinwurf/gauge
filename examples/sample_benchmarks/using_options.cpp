// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <numeric>

#include <gauge/gauge.hpp>

// This test shows how you can add options to the benchmark
// options
class using_options : public gauge::time_benchmark
{
public:

    void setup()
    {
        // Setup the test this could also be done inside the
        // BENCHMARK macro
        gauge::config_set cs = get_current_configuration();

        uint32_t size = cs.get_value<uint32_t>("array_size");

        m_vector.resize(size);

        std::generate(m_vector.begin(), m_vector.end(), rand);
    }

    void get_options(gauge::po::variables_map& options)
    {
        // The options specified are available by overriding the
        // get_options() function and using the option name and
        // type to access the variables_map.
        auto values = options["array_size"].as<std::vector<int> >();
        for (auto v : values)
        {
            gauge::config_set cs;
            cs.set_value<uint32_t>("array_size", v);
            add_configuration(cs);
        }
    }

    std::vector<uint32_t> m_vector;

};

// Using this macro we may specify options. For specifying options
// we use the boost program options library. So you may additional
// details on how to do it in the manual for that library.
BENCHMARK_OPTION(array_size)
{
    gauge::po::options_description options;

    std::vector<int> size = {10U, 40U};

    auto default_size =
        gauge::po::value<std::vector<int> >()->default_value(
            size, "")->multitoken();

    options.add_options()
        ("array_size", default_size, "Set bit array size");

    gauge::runner::instance().register_options(options);
}

// Counting bits set, Brian Kernighan's way
inline uint32_t count_bk(uint32_t v)
{
    uint32_t c; // c accumulates the total bits set in v

    for (c = 0; v; c++)
    {
        v &= v - 1; // clear the least significant bit set
    }
    return c;
}

// Counting bits set (naive way)
inline uint32_t count_naive(uint32_t v)
{
    uint32_t c; // c accumulates the total bits set in v

    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    return c;
}

BENCHMARK_F_INLINE(using_options, CountBits, count_bk, 10)
{
    volatile uint32_t sum;

    RUN
    {
        for(auto v: m_vector)
            sum += count_bk(v);
    }

    (void) sum; // Suppress warning about unused variable
}

BENCHMARK_F_INLINE(using_options, CountBits, count_naive, 10)
{
    volatile uint32_t sum;

    RUN
    {
        for(auto v: m_vector)
            sum += count_naive(v);
    }

    (void) sum; // Suppress warning about unused variable
}
