// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdio>

#include <gtest/gtest.h>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include <gauge/gauge.hpp>

namespace bc = boost::chrono;

double get_micro(bc::high_resolution_clock::duration delta)
{
    return (double)bc::duration_cast<bc::microseconds>(delta).count();
}

struct sleep_benchmark : public gauge::time_benchmark
{
    double measurement()
    {
        // Get the time spent per iteration
        double time = gauge::time_benchmark::measurement();

        printf("Sleep time: %.3f us\n", time);
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

BENCHMARK_F(sleep_benchmark, Gauge, sleep_10usecs, 1)
{
     run_benchmark(10);
}

BENCHMARK_F(sleep_benchmark, Gauge, sleep_100usecs, 1)
{
    run_benchmark(100);
}

BENCHMARK_F(sleep_benchmark, Gauge, sleep_1000usecs, 1)
{
    run_benchmark(1000);
}

BENCHMARK_F(sleep_benchmark, Gauge, sleep_10000usecs, 1)
{
    run_benchmark(10000);
}

BENCHMARK_F(sleep_benchmark, Gauge, sleep_100000usecs, 1)
{
    run_benchmark(100000);
}

TEST(Gauge, sleep_1000us)
{
    bc::high_resolution_clock::time_point t1, t2;
    t1 = bc::high_resolution_clock::now();
    boost::this_thread::sleep_for(bc::microseconds(50));
    t2 = bc::high_resolution_clock::now();
    double sleep_time = get_micro(t2 - t1);
    printf("Isolated sleep time:  %.3f us\n", sleep_time);

    int argc = 1;
    const char* argv[] = { "program" };

    gauge::runner::add_default_printers();
    gauge::runner::run_benchmarks(argc, argv);
}
