#ifndef GAUGE_TIME_BENCHMARK_HPP
#define GAUGE_TIME_BENCHMARK_HPP

#include <memory>

#include "benchmark.hpp"

namespace gauge
{
    /// Base class for benchmarks measuring time
    class time_benchmark : public benchmark
    {
    public:

        time_benchmark();
        ~time_benchmark();

        virtual void init();

        virtual uint64_t iteration_count() const;

        virtual void start();

        virtual void stop();

        virtual double measurement();

        virtual bool accept_measurement();

        std::string unit_text() const;

    private:

        class impl;
        std::unique_ptr<impl> m_impl;
    };
}

#endif

