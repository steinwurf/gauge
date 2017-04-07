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
        // This should be higher than 99% of the requested time
        // The sleep period might end a little earlier on Windows
        EXPECT_GE(time, m_delay.count() * 0.99);
        return time;
    }

    void run_benchmark(bc::microseconds delay)
    {
        m_delay = delay;
        RUN
        {
            boost::this_thread::sleep_for(delay);
        }
    }

protected:

    bc::microseconds m_delay;
};

BENCHMARK_F_INLINE(sleep_benchmark, Sleep, 10usec, 1)
{
    run_benchmark(bc::microseconds(10));
}

BENCHMARK_F_INLINE(sleep_benchmark, Sleep, 100usec, 1)
{
    run_benchmark(bc::microseconds(100));
}

BENCHMARK_F_INLINE(sleep_benchmark, Sleep, 1msec, 1)
{
    run_benchmark(bc::milliseconds(1));
}

BENCHMARK_F_INLINE(sleep_benchmark, Sleep, 10msec, 1)
{
    run_benchmark(bc::milliseconds(10));
}

BENCHMARK_F_INLINE(sleep_benchmark, Sleep, 100msec, 1)
{
    run_benchmark(bc::milliseconds(100));
}

TEST(Gauge, sleep_intervals)
{
    // Dummy arguments for gauge::runner
    int argc = 1;
    const char* argv[] = { "program" };

    gauge::runner::add_default_printers();
    gauge::runner::run_benchmarks(argc, argv);
}
