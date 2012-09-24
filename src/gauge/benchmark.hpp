#ifndef GAUGE_BENCHMARK_HPP
#define GAUGE_BENCHMARK_HPP

#include <cassert>
#include <vector>

#include "config_set.hpp"

namespace gauge
{
    /// Base class for all benchmark.
    class benchmark
    {
    public:

        /// @param benchmark_id sets the benchmark id
        void set_id(uint32_t benchmark_id)
            {
                m_id = benchmark_id;
            }

        /// @return the benchmark id
        uint32_t id() const
            {
                return m_id;
            }

        /// @return the test case name
        virtual std::string testcase_name() const
            { return "unknown"; }

        /// @return the name of the benchmark in a test case
        virtual std::string benchmark_name() const
            { return "unknown"; }

        /// @todo: consider to handle this differently since
        /// we now allow the user to override the number of
        /// runs it could be confusing why runs() does not return
        /// the actual number of runs completed..
        /// @return the number of runs to be completed for this benchmark
        virtual uint32_t runs() const = 0;

        /// Reset the state of a measurement controller
        virtual void init(){};

        /// For how many iterations should we loop
        /// @return the iteration count
        virtual uint64_t iteration_count() const
            { return 1; };

        /// @return the number of configurations create for this
        ///         benchmark
        uint32_t configuration_count() const
            { return m_configurations.size(); }

        /// @return true if the benchmark has configurations
        bool has_configurations() const
            { return configuration_count() > 0; }

        /// Updates the configuration index
        /// @param config_index, the new configuration index
        void set_current_configuration(uint32_t config_index)
            {
                assert(config_index < configuration_count());
                m_config_index = config_index;
            }

        /// Updates the configuration index
        /// @param config_index, the new configuration index
        void add_configuration(const config_set &config)
            {
                m_configurations.push_back(config);
            }

        /// @return the current configuration index
        uint32_t current_configuration() const
            {
                assert(has_configurations());
                return m_config_index;
            }

        /// @return the current configuration set if this
        ///         function is not overridden by a sub-class
        ///         a default config_set is returned
        config_set get_current_configuration() const
            {
                assert(has_configurations());
                assert(m_config_index < m_configurations.size());

                return m_configurations[m_config_index];
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

        /// The benchmark id given by the static call to
        /// register_id in the runner
        uint32_t m_id;

        /// Stores the current configuration index
        uint32_t m_config_index;

        /// Stores the different configurations
        std::vector<config_set> m_configurations;
    };
}

#endif

