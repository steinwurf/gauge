#ifndef GAUGE_BENCHMARK_H
#define GAUGE_BENCHMARK_H

#include <cassert>
#include "config_set.h"

namespace gauge
{

    /// Base class for all benchmark.
    class benchmark
    {
    public:

        /// @return the test case name
        virtual std::string testcase_name() const
            { return "unknown"; }

        /// @return the name of the benchmark in a test case
        virtual std::string benchmark_name() const
            { return "unknown"; }

        /// @return the number of runs to be completed for this benchmark
        virtual uint32_t runs() const = 0;

        /// Reset the state of a measurement controller
        virtual void init(){};

        /// For how many iterations should we loop
        /// @return the iteration count
        virtual uint32_t iteration_count() const
            { return 1; };

        /// @return the number of configurations create for this
        ///         benchmark
        virtual uint32_t configuration_count() const
            { return 0; }

        /// @return true if the benchmark has configurations
        virtual bool has_configurations() const
            { return configuration_count() > 0; }

        /// Updates the configuration index
        /// @param config_index, the new configuration index
        virtual void set_configuration(uint32_t config_index)
            {
                assert(config_index < configuration_count());
                m_config_index = config_index;
            }

        /// @return the current configuration index
        virtual uint32_t current_configuration() const
            {
                assert(has_configurations());
                return m_config_index;
            }

        /// @return the current configuration set if this
        ///         function is not overridden by a sub-class
        ///         a default config_set is returned
        virtual config_set configuration_set() const
            {
                config_set cs;
                cs.set_value("configuration_id", current_configuration());
                return cs;
            }

        /// Starts a measurement
        virtual void start() = 0;

        /// Stops a measurement
        virtual void stop() = 0;

        /// @return true if a warm-up iteration is needed
        virtual bool needs_warmup_iteration(){ return false; }

        /// @return true if the previous measurement was accepted
        virtual bool accept_measurement(){ return true; }

        /// @return the result of the latest measurement
        virtual double measurement() = 0;

        /// Returns the unit we are measuring
        virtual std::string unit_text() const = 0;

        /// Implemented by the user contains the actual test
        /// When a user uses the BENCHMARK{ code-section } the
        /// code-section contains the guts of the test_body
        virtual void test_body() = 0;

        /// Called once before every test run. Allows a user to
        /// prepare resources for the upcoming benchmark
        virtual void setup(){}

        /// Called once after every test run. Allows a user to
        /// cleanup resources in between benchmark runs.
        virtual void tear_down()
            {}

    private:

        /// Stores the current configuration index
        uint32_t m_config_index;

    };


}

#endif


