#ifndef GAUGE_RESULT_HPP
#define GAUGE_RESULT_HPP

#include <vector>
#include <cstdint>

namespace gauge
{
    /// Struct containing the benchmark results from a single
    /// test run.
    ///
    /// The results is give so that m_results[0] denotes the
    /// result obtained for run 0 and m_iterations[0] gives
    /// number of iterations performed to obtain m_results[0]
    struct result
    {
        /// The result per iteration
        std::vector<double> m_results;

        /// The number of iterations performed to obtain the
        /// corresponding result
        std::vector<uint64_t> m_iterations;
    };
}

#endif

