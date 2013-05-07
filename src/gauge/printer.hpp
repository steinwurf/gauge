#pragma once

#include "benchmark.hpp"
#include "results.hpp"

namespace gauge
{

    /// Progress printer interface. To provide a custom
    /// Result printer create a sub-class of this interface
    /// and add it to gauge using:
    ///
    ///   gauge::runner::instance().printers().push_back(
    ///     std::make_shared<my_custom_printer>());
    ///
    class printer
    {
    public:

        /// Destructor
        ~printer()
        { }

        /// Called when the benchmark program is started
        virtual void start_benchmark()
        { }

        /// Called when a result from a benchmark is ready
        /// @param info The benchmark
        /// @param result The benchmark results
        virtual void benchmark_result(const benchmark &/*info*/,
                                      const table &/*results*/)
        { }

        /// Add options to the available commandline arguments
        virtual void set_options(po::variables_map& /*options*/)
        { }

        /// Called when the benchmark program is finished
        virtual void end_benchmark()
        { }

    };

}

