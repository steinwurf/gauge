#include <gauge/gauge.hpp>
#include <cstdlib>

class benchmark_me
{
public:
    benchmark_me()
        : m_count(0),
          m_limit(1000)
    {}

    void run_this()
    {
        uint32_t k = 0;
        while(k < m_limit)
        {
            k += rand() % 100;
            ++m_count;
        }
    }

    uint32_t m_limit;
    uint32_t m_count;
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

    custom_measurement()
        : m_counts("counts"),
          m_steps("step size")
    {
        m_counts.add_row("counts");
        m_steps.add_row("steps");
    }

    void start()
    {
        m_old_count = m_benchmark_me.m_count;
    }

    void stop()
    {
        m_new_count = m_benchmark_me.m_count;
        m_limit = m_benchmark_me.m_limit;
    }

    double measurement()
    {
        assert(m_new_count >= m_old_count);
        uint32_t diff = m_new_count - m_old_count;

        return static_cast<double>(diff);
    }

    /// The unit we measure in
    std::string unit_text() const
    { return "counts"; }

    void store_run()
    {
        m_counts.add_run(iteration_count());
        m_steps.add_run(iteration_count());

        double diff = static_cast<double>(m_new_count - m_old_count);

        m_counts["counts"] = diff;
        m_steps["steps"] = m_limit / diff;
    }

    void store_table(std::vector<gauge::table> &results)
    {
        results.push_back(m_counts);
        results.push_back(m_steps);
    }


    /// The class being bench-marked
    benchmark_me m_benchmark_me;

    uint32_t m_old_count;
    uint32_t m_new_count;
    uint32_t m_limit;

    gauge::table m_counts;
    gauge::table m_steps;
};


BENCHMARK_F(custom_measurement, BenchmarkMe, RunCount, 100)
{
    RUN{
        m_benchmark_me.run_this();
    }
}


