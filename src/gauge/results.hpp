// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace gauge
{
/// Struct containing the benchmark results from a single
/// test run.
///
/// The results is give so that m_results[0] denotes the
/// result obtained for run 0 and m_iterations[0] gives
/// number of iterations performed to obtain m_results[0]
struct results
{
    /// The name of this result - describes the result type e.g.
    /// "time" or "throughput"
    std::string m_name;

    /// The unit of this result e.g. seconds or MB/s
    std::string m_unit;

    /// The result per iteration
    std::vector<double> m_results;

    /// The number of iterations performed to obtain the
    /// corresponding result
    std::vector<uint64_t> m_iterations;
};

struct result
{
    /// The name of this result - describes the result type e.g.
    /// "time" or "throughput"
    std::string m_name;

    /// The unit of this result e.g. seconds or MB/s
    std::string m_unit;

    /// The result per iteration
    std::vector<double> m_results;

    /// The number of iterations performed to obtain the
    /// corresponding result
    std::vector<uint64_t> m_iterations;
};

typedef std::map<std::string, result> temp_results;

}
