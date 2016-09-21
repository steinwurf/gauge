// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>

#include "benchmark.hpp"

namespace gauge
{
    /// Base class for benchmarks measuring time
    class time_benchmark : public benchmark
    {
    public:

        /// Constructor
        time_benchmark();

        /// Destructor
        ~time_benchmark();

    public:
        // From benchmark

        /// @copydoc benchmark::init()
        virtual void init();

        /// @copydoc benchmark::iteration_count() const
        virtual uint64_t iteration_count() const;

        /// @copydoc benchmark::start()
        virtual void start();

        /// @copydoc benchmark::stop()
        virtual void stop();

        /// @copydoc benchmark::measurement()
        virtual double measurement();

        /// @copydoc benchmark::accept_measurement()
        virtual bool accept_measurement();

        /// @copydoc benchmark::unit_text() const
        virtual std::string unit_text() const;

        virtual void store_run(tables::table& results);

    private:

        class impl;
        std::unique_ptr<impl> m_impl;
    };
}
