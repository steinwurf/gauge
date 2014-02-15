// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <tables/table.hpp>

#include <boost/program_options.hpp>

#include "benchmark.hpp"
#include "results.hpp"

namespace gauge
{

    namespace po = boost::program_options;

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

        /// Create a new printer
        /// @param name The name of the printer
        /// @param default_filename The default name of the outputted file
        /// without the file extension
        printer(const std::string& name, bool enabled = true);

        // Returns true if the printer is enabled.
        bool is_enabled() const;

        /// Add options to the available commandline arguments
        virtual void set_options(po::variables_map& options);

        /// Called when the benchmark program is started
        virtual void start()
        { }

        /// Called when a specific benchmark is started
        virtual void start_benchmark()
        { }

        /// Called when a result from a benchmark is ready
        /// @param info The benchmark
        /// @param result The benchmark results
        virtual void benchmark_result(const benchmark &/*info*/,
                                      const tables::table &/*results*/)
        { }

        /// Called when a specific benchmark is finished
        virtual void end_benchmark()
        { }

        /// Called when the benchmark program is finished
        virtual void end()
        { }

    protected:

        /// Name of the printer
        std::string m_name;

        /// Boolean determining wether the printer is enabled or not.
        bool m_enabled;

    };

}
