#pragma once

#include <vector>
#include <memory>

#include <boost/program_options.hpp>

#include "benchmark.hpp"
#include "printer.hpp"

namespace gauge
{
    namespace po = boost::program_options;

    /// The gauge benchmark runner. Responsible for
    /// invoking the registered benchmarks.
    class runner
    {
    public:

        /// Pointer to a benchmark
        typedef std::shared_ptr<benchmark> benchmark_ptr;

        /// Pointer to a result printer
        typedef std::shared_ptr<printer> printer_ptr;

        /// Make benchmark function
        typedef std::function<benchmark_ptr ()> make_benchmark;

        /// Creates a new runner with a default printer
        runner();

        /// Access to the gauge benchmark runner instance. Even though we
        /// don't like singletons we use it here since it reduces complexity
        /// of registering and running benchmarks
        /// @return instance of the gauge benchmark runner
        static runner& instance();

        /// Run all the benchmarks registered with the gauge benchmark
        /// runner
        static void run_benchmarks(int argc = 0, const char* argv[] = 0);

        /// Registers an unique id for each benchmark type. All benchmarks
        /// call this function once to get their id.
        /// @return the unique id for a benchmark.
        static uint32_t register_id();

        /// Creates the desired benchmark with the runner
        template<class T>
        void register_benchmark(std::string testcase_name,
                                std::string benchmark_name)
        {
            uint32_t id = T::benchmark_id();

            make_benchmark benchmark = std::bind(std::make_shared<T>);
            add_benchmark(id, benchmark, testcase_name, benchmark_name);
        }

        /// Fetch the options for a specific benchmark
        template<class T>
        void register_options()
        {
            T::add_options();
        }

        /// Stores the registered options with the runner
        void register_options(const po::options_description &options);

        /// Adds a new benchmark
        // void add_benchmark(uint32_t id, benchmark_ptr benchmark);
        void add_benchmark(uint32_t id,
                           make_benchmark benchmark,
                           std::string testcase_name,
                           std::string benchmark_name);

        /// Returns the benchmark with the specific id
        /// @param id of the desired benchmark
        /// @return pointer to the benchmark
        //benchmark_ptr get_benchmark(uint32_t id);

        /// Returns the id of the currently active benchmark
        /// @return id of benchmark
        benchmark_ptr current_benchmark();

        /// Start a new benchmark runner using the commandline
        /// parameters specified. Exceptions are not handled.
        /// @param argc for the program
        /// @param argv for the program
        void run_unsafe(int argc = 0, const char *argv[] = 0);

        /// Start a new benchmark runner using the commandline
        /// parameters specified.
        /// @param argc for the program
        /// @param argv for the program
        void run(int argc = 0, const char *argv[] = 0);

        /// Run all the benchmarks
        void run_all();

        /// Run the benchmarks matching the specified filter
        /// @param filter name e.g. MyTest.*
        void run_single_filter(const std::string &filter);

        /// Run the benchmarks matching the specified filter given as a
        /// comma separated file.
        /// @param filter name e.g. MyTest.* MyOtherTest.Yir
        void run_all_filters(const std::vector<std::string> &filters);

        /// Runs the benchmark with the specified id
        void run_benchmark(benchmark_ptr bench);

        /// Runs the specified benchmark running through the available
        /// configurations
        void run_benchmark_configurations(benchmark_ptr bench);

        /// @return access to the runners printers
        std::vector<printer_ptr>& printers();

        /// Parse the add_column options
        /// @param column Value from the input options
        void parse_add_column(const std::string &option);

    private:

        struct impl;
        std::unique_ptr<impl> m_impl;
    };
}
