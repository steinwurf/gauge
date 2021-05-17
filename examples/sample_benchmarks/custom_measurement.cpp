// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdlib>
#include <gauge/gauge.hpp>
#include <tables/table.hpp>

class benchmark_me
{
public:
    benchmark_me() : m_count(0), m_limit(1000)
    {
    }

    void run_this()
    {
        uint32_t k = 0;
        while (k < m_limit)
        {
            k += rand() % 100;
            ++m_count;
        }
    }

    uint32_t m_count;
    uint32_t m_limit;
};

/// A custom benchmark where we do not measure time
/// but implement our own start(), stop() and
/// measurement() functions.
///
/// In this case we want to see how many counts we need
/// in the benchmark_me class
class custom_measurement : public gauge::benchmark
{
public:
    void start()
    {
        m_old_count = m_benchmark_me.m_count;
    }

    void stop()
    {
        m_new_count = m_benchmark_me.m_count;
    }

    double measurement()
    {
        assert(m_new_count >= m_old_count);
        uint32_t diff = m_new_count - m_old_count;

        return static_cast<double>(diff);
    }

    /// The unit we measure in
    std::string unit_text() const
    {
        return "counts";
    }

    void store_run(tables::table& results)
    {
        if (!results.has_column("counts"))
            results.add_column("counts");

        results.set_value("counts", measurement());
    }

    void test_body() final
    {
        RUN
        {
            m_benchmark_me.run_this();
        }
    }

    /// The class being bench-marked
    benchmark_me m_benchmark_me;

    uint32_t m_old_count;
    uint32_t m_new_count;
};

BENCHMARK_F(custom_measurement, BenchmarkMe, RunCount, 100);
