#ifndef GAUGE_TIME_BENCHMARK_H
#define GAUGE_TIME_BENCHMARK_H

#include "benchmark.h"
#include <memory>

namespace gauge
{

    /// Base class for benchmarks measuring time
    class time_benchmark : public benchmark
    {
    public:

        time_benchmark();
        ~time_benchmark();

        virtual void init();

        virtual uint32_t iteration_count() const;

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


