// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gauge/gauge.hpp>

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

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

    void run_benchmark(std::chrono::microseconds delay)
    {
        m_delay = delay;
        RUN
        {
            std::this_thread::sleep_for(delay);
        }
    }

protected:

    std::chrono::microseconds m_delay;
};

BENCHMARK_F_INLINE(sleep_benchmark, sleep, 10usec, 1)
{
    run_benchmark(std::chrono::microseconds(10));
}

BENCHMARK_F_INLINE(sleep_benchmark, sleep, 100usec, 1)
{
    run_benchmark(std::chrono::microseconds(100));
}

BENCHMARK_F_INLINE(sleep_benchmark, sleep, 1msec, 1)
{
    run_benchmark(std::chrono::milliseconds(1));
}

BENCHMARK_F_INLINE(sleep_benchmark, sleep, 10msec, 1)
{
    run_benchmark(std::chrono::milliseconds(10));
}

BENCHMARK_F_INLINE(sleep_benchmark, sleep, 100msec, 1)
{
    run_benchmark(std::chrono::milliseconds(100));
}
