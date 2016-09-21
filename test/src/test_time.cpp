// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gauge/gauge.hpp>

#include <gtest/gtest.h>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

namespace bc = boost::chrono;

struct sleep_benchmark : public gauge::time_benchmark
{
    double measurement()
    {
        // Get the time spent sleeping
        double time = gauge::time_benchmark::measurement();
        // This should be higher than the requested time
        EXPECT_GT(time, m_last_delay);
        return time;
    }

    void run_benchmark(uint32_t delay)
    {
        m_last_delay = delay;
        RUN
        {
            boost::this_thread::sleep_for(bc::microseconds(delay));
        }
    }

protected:

    uint32_t m_last_delay;
};

BENCHMARK_F_INLINE(sleep_benchmark, Gauge, sleep_10usecs, 1)
{
    run_benchmark(10);
}

BENCHMARK_F_INLINE(sleep_benchmark, Gauge, sleep_100usecs, 1)
{
    run_benchmark(100);
}

BENCHMARK_F_INLINE(sleep_benchmark, Gauge, sleep_1000usecs, 1)
{
    run_benchmark(1000);
}

BENCHMARK_F_INLINE(sleep_benchmark, Gauge, sleep_10000usecs, 1)
{
    run_benchmark(10000);
}

BENCHMARK_F_INLINE(sleep_benchmark, Gauge, sleep_100000usecs, 1)
{
    run_benchmark(100000);
}

TEST(Gauge, sleep_intervals)
{
    // Dummy arguments for gauge::runner
    int argc = 1;
    const char* argv[] = { "program" };

    gauge::runner::add_default_printers();
    gauge::runner::run_benchmarks(argc, argv);
}


// TEST(Gauge, reference_sleep_intervals)
// {
//     uint32_t loop = 100000;

//     auto start = bc::high_resolution_clock::now();

//     for(uint32_t i = 0; i < loop; ++i)
//         boost::this_thread::sleep_for(bc::microseconds(10));

//     auto stop = bc::high_resolution_clock::now();

//     auto duration = static_cast<double>(
//         bc::duration_cast<bc::microseconds>(stop - start).count());

//     std::cout << "Duration " << duration << std::endl;
//     std::cout << "Duration per loop" << duration/loop << std::endl;
// }
