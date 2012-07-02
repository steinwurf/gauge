#ifndef GAUGE_ITERATION_CONTROLLER_H
#define GAUGE_ITERATION_CONTROLLER_H

#include "benchmark.h"
#include "runner.h"

namespace gauge
{

    /// Iteration controller used by the benchmarks to
    /// control the measurement loop.
    class iteration_controller
    {
    public:

        /// Pointer to the benchmark
        typedef std::shared_ptr<benchmark> benchmark_ptr;

    public:

        /// Create a new iteration controller for the benchmark
        /// with a specific id.
        /// Starts the benchmark
        /// @param benchmark_id the id of the benchmark
        iteration_controller()
            : m_iteration_count(0),
              m_total_iterations(0),
              m_benchmark(gauge::runner::instance().current_benchmark())
            {
                assert(m_benchmark);

                m_total_iterations = m_benchmark->iteration_count();

                m_benchmark->start();
            }

        /// Stops the benchmark
        ~iteration_controller()
            {
                m_benchmark->stop();
            }

        /// @return true if the required number of iterations has been
        ///         completed
        bool is_done()
            {
                return m_iteration_count > m_total_iterations;
            }

        /// Increments the iteration counter
        void next()
            {
                ++m_iteration_count;
            }

    private:

        /// The current iteration count
        uint32_t m_iteration_count;

        /// The total number of iterations to perform
        uint32_t m_total_iterations;

        /// Pointer to the benchmark
        benchmark_ptr m_benchmark;

    };

}

#endif


