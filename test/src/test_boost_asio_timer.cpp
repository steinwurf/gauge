// Copyright (c) 2016 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gauge/gauge.hpp>

#include <gtest/gtest.h>

#include <boost/chrono.hpp>
#include <boost/asio.hpp>

struct boost_asio_timer_benchmark : public gauge::time_benchmark
{
    using clock_type = boost::chrono::high_resolution_clock;

    bool accept_measurement()
    {
        // Force only one iteration
        return true;
    }

    double measurement()
    {
        // Get the time spent sleeping
        double time = gauge::time_benchmark::measurement();
        // This should be higher than 99% of the requested time
        // The timer might expire a little earlier on Windows
        EXPECT_GE(time, m_delay.count() * 0.99);
        return time;
    }

    void run_benchmark(boost::chrono::microseconds delay)
    {
        m_delay = delay;

        boost::asio::basic_waitable_timer<clock_type> timer(m_io);

        RUN
        {
            timer.expires_from_now(m_delay);
            timer.async_wait(
                std::bind(&boost_asio_timer_benchmark::timeout_handler,
                          this,
                          std::placeholders::_1));

            m_io.run();
            // reset the io service in order for it to work on the next run.
            m_io.reset();
        }

    }

    void timeout_handler(const boost::system::error_code& ec)
    {
        (void)ec;
    }

protected:

    boost::asio::io_service m_io;

    boost::chrono::microseconds m_delay;
};

BENCHMARK_F_INLINE(boost_asio_timer_benchmark, AsioTimer, 100msec, 1)
{
    run_benchmark(boost::chrono::milliseconds(100));
}

BENCHMARK_F_INLINE(boost_asio_timer_benchmark, AsioTimer, 10msec, 1)
{
    run_benchmark(boost::chrono::milliseconds(10));
}

BENCHMARK_F_INLINE(boost_asio_timer_benchmark, AsioTimer, 1msec, 1)
{
    run_benchmark(boost::chrono::milliseconds(1));
}

BENCHMARK_F_INLINE(boost_asio_timer_benchmark, AsioTimer, 10usec, 1)
{
    run_benchmark(boost::chrono::microseconds(10));
}

TEST(Gauge, boost_asio_timer)
{
    // Dummy arguments for gauge::runner
    int argc = 1;
    const char* argv[] = { "program" };

    gauge::runner::run_benchmarks(argc, argv);
}
