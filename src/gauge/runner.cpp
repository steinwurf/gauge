// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cassert>
#include <cstdlib>
#include <string>
#include <map>
#include <limits>
#include <vector>

#include <boost/program_options.hpp>

#include "console_printer.hpp"
#include "csv_printer.hpp"
#include "json_printer.hpp"
#include "python_printer.hpp"
#include "stdout_printer.hpp"
#include "results.hpp"

#include "runner.hpp"

namespace gauge
{
    struct runner::impl
    {
        /// Benchmark container
        typedef std::map<std::string, uint32_t> benchmark_map;

        /// Test case container
        typedef std::map<std::string, benchmark_map> testcase_map;

        /// The registered benchmarks
        // std::map<uint32_t, benchmark_ptr> m_benchmarks;
        std::map<uint32_t, make_benchmark> m_benchmarks;

        /// Container for all the registered printers.
        std::vector<printer_ptr> m_printers;

        /// Test case map
        testcase_map m_testcases;

        /// The available program options
        boost::program_options::options_description m_options_description;

        /// Parsed program options
        po::variables_map m_options;

        /// The currently active benchmark
        benchmark_ptr m_current_benchmark;

        /// Custom columns
        std::map<std::string, std::string> m_columns;
    };

    runner::runner()
        : m_impl(new runner::impl())
    { }

    runner& runner::instance()
    {
        static runner singleton;
        return singleton;
    }

    void runner::add_default_printers()
    {
        instance().printers().push_back(
            std::make_shared<gauge::console_printer>());

        instance().printers().push_back(
            std::make_shared<gauge::python_printer>());

        instance().printers().push_back(
            std::make_shared<gauge::json_printer>());

        instance().printers().push_back(
            std::make_shared<gauge::csv_printer>());

        instance().printers().push_back(
            std::make_shared<gauge::stdout_printer>());
    }

    void runner::run_benchmarks(int argc, const char* argv[])
    {
        runner& run = instance();
        run.run(argc, argv);
    }

    void runner::register_options(const po::options_description &options)
    {
        assert(m_impl);

        const auto& opt = options.options();
        for (const auto& o: opt)
            m_impl->m_options_description.add(o);
    }

    uint32_t runner::register_id()
    {
        // We start from one so we let 0 be an invalid benchmark id
        static uint32_t id = 1;

        // If zero is invalid we cannot have overflow, but that should
        // not be a problem - anyways better safe than sorry
        assert(id < std::numeric_limits<uint32_t>::max());
        return ++id;
    }

    void runner::add_benchmark(uint32_t id,
                               make_benchmark benchmark,
                               std::string testcase_name,
                               std::string benchmark_name)
    {
        assert(m_impl);

        m_impl->m_benchmarks[id] = benchmark;
        m_impl->m_testcases[testcase_name][benchmark_name] = id;
    }

    runner::benchmark_ptr runner::current_benchmark()
    {
        assert(m_impl->m_current_benchmark);
        return m_impl->m_current_benchmark;
    }

    void runner::run(int argc, const char *argv[])
    {
        try
        {
            run_unsafe(argc, argv);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void runner::run_unsafe(int argc, const char *argv[])
    {
        assert(m_impl);

        po::options_description options("Gauge");

        options.add_options()
            ("help", "produce help message")
            ("print_tests", "print testcases")
            ("print_benchmarks", "print benchmarks")
            ("result_filter",
             po::value<std::vector<std::string> >()->multitoken(),
             "Filter which results should be stored "
              "for example ./benchmark --result_filter=time multiple filters "
              "can be a comma separated list of filters e.g. "
              "--result_filter=time throughput")
            ("gauge_filter",
             po::value<std::vector<std::string> >()->multitoken(),
             "Filter which test-cases or benchmarks to run based on their name "
              "for example ./benchmark --gauge_filter=MyTest.* or "
              "--gauge_filter=*.MyBenchmark or even --gauge_filter=*.* "
              "Multiple filters can also be specified e.g. "
              "--gauge_filter=MyTest.one MyTest.two")
            ("runs", po::value<uint32_t>(),
             "Sets the number of runs to complete. Overrides the "
             "settings specified in the benchmark ex. --runs=50")
            ("add_column",
             po::value<std::vector<std::string> >()->multitoken(),
             "Add a column to the test results, this can be use to "
             "add custom information to the result files "
             "./benchmark --add_column cpu=i7 "
             "\"date=Monday 1st June 2021\"")
            ("dry_run",
             "Initializes the benchmark without running it. This is useful to "
             "check whether the right command-line arguments have been passed "
             "to the benchmark executable.");

        options.add(m_impl->m_options_description);

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, options), vm);
        po::notify(vm);

        m_impl->m_options = vm;


        if (m_impl->m_options.count("help"))
        {
            std::cout << options << std::endl;
            return;
        }

        if (m_impl->m_options.count("print_tests"))
        {
            for (const auto& testcase : m_impl->m_testcases)
            {
                std::cout << testcase.first << " ";
            }
            std::cout << std::endl;
            return;
        }

        if (m_impl->m_options.count("print_benchmarks"))
        {
            for (const auto& testcase : m_impl->m_testcases)
            {
                for (const auto& benchmark : testcase.second)
                {
                    std::cout << testcase.first << "."
                              << benchmark.first << std::endl;
                }
            }
            return;
        }

        if (m_impl->m_options.count("add_column"))
        {
            auto v = m_impl->m_options["add_column"].as<
                std::vector<std::string> >();
            for (const auto& s : v)
            {
                parse_add_column(s);
            }
        }

        // Deliver possible options to printers and start them
        for (auto& printer: m_impl->m_printers)
        {
            printer->set_options(m_impl->m_options);
        }

        // Notify all printers that we are starting
        for (auto& printer: enabled_printers())
        {
            printer->start();
        }
        // Check whether we should run all tests or whether we
        // should use a filter
        if (m_impl->m_options.count("gauge_filter"))
        {
            auto f = m_impl->m_options["gauge_filter"]
                .as<std::vector<std::string>>();
            run_all_filters(f);
        }
        else
        {
            run_all();
        }

        // Notify all printers that we are done
        for (auto& printer: enabled_printers())
        {
            printer->end();
        }
    }

    void runner::parse_add_column(const std::string &option)
    {
        std::istringstream sstream(option);

        std::string column_name;
        std::string column_value;

        std::getline(sstream, column_name, '=');

        if (!sstream)
            throw std::runtime_error("Error malformed add_column"
                                     " (example cpu=i7)");

        std::getline(sstream, column_value);

        if (!sstream)
        {
            throw std::runtime_error("Error malformed add_column"
                                     " (example cpu=i7)");
        }

        assert(m_impl->m_columns.find(column_name) ==
               m_impl->m_columns.end());

        m_impl->m_columns[column_name] = column_value;
    }

    void runner::run_all()
    {
        assert(m_impl);

        for (auto& m : m_impl->m_benchmarks)
        {
            auto& make = m.second;
            auto benchmark = make();

            assert(benchmark);

            run_benchmark_configurations(benchmark);
        }
    }

    void runner::run_all_filters(const std::vector<std::string> &filters)
    {
        for (const auto& f : filters)
        {
            run_single_filter(f);
        }
    }

    void runner::run_single_filter(const std::string &filter)
    {
        std::istringstream sstream(filter);

        std::string testcase_name;
        std::string benchmark_name;

        std::getline(sstream, testcase_name, '.');

        if (!sstream)
            throw std::runtime_error("Error malformed gauge_filter"
                                     " (example MyTest.*)");

        std::getline(sstream, benchmark_name);

        if (!sstream)
        {
            throw std::runtime_error("Error malformed gauge_filter"
                                     " (example MyTest.*)");
        }

        // Evaluate all possible filter combinations
        if (testcase_name == "*" && benchmark_name == "*")
        {
            run_all();
        }
        else if (testcase_name == "*")
        {
            // The benchmark must be run for each of the testcases for which
            // it belongs. If the requested benchmark is not found, throw an
            // error

            bool benchmark_found = false;

            for (const auto& testcase : m_impl->m_testcases)
            {
                for (const auto& b : testcase.second)
                {
                    if (benchmark_name == b.first)
                    {

                        uint32_t id = b.second;
                        assert(m_impl->m_benchmarks.find(id) !=
                               m_impl->m_benchmarks.end());

                        auto& make = m_impl->m_benchmarks[id];
                        auto benchmark = make();
                        run_benchmark_configurations(benchmark);
                        benchmark_found = true;
                    }
                }
            }

            if (!benchmark_found)
            {
                throw std::runtime_error("Error benchmark not found");
            }
        }
        else if (benchmark_name == "*")
        {
            // All the benchmarks from a testcase must be run. If the requested
            // testcase is not found, throw an error

            if (m_impl->m_testcases.find(testcase_name) ==
               m_impl->m_testcases.end())
            {
                throw std::runtime_error("Error testcase not found");
            }

            auto &benchmarks = m_impl->m_testcases[testcase_name];

            for (auto& b : benchmarks)
            {
                uint32_t id = b.second;

                assert(m_impl->m_benchmarks.find(id) !=
                       m_impl->m_benchmarks.end());

                auto& make = m_impl->m_benchmarks[id];
                auto benchmark = make();

                run_benchmark_configurations(benchmark);
            }
        }
        else
        {
            // Run the specific testcase_name.benchmark_name pair
            if (m_impl->m_testcases.find(testcase_name) ==
               m_impl->m_testcases.end())
            {
                throw std::runtime_error("Error testcase not found");
            }

            auto &benchmarks = m_impl->m_testcases[testcase_name];

            if (benchmarks.find(benchmark_name) == benchmarks.end())
            {
                throw std::runtime_error("Error benchmark not found");
            }

            uint32_t id = benchmarks.find(benchmark_name)->second;

            assert(m_impl->m_benchmarks.find(id) !=
                   m_impl->m_benchmarks.end());

            auto& make = m_impl->m_benchmarks[id];
            auto benchmark = make();

            run_benchmark_configurations(benchmark);
        }
    }

    void runner::run_benchmark_configurations(benchmark_ptr benchmark)
    {
        assert(benchmark);
        assert(m_impl);

        benchmark->get_options(m_impl->m_options);

        if (benchmark->has_configurations())
        {
            uint32_t configs = benchmark->configuration_count();

            for (uint32_t i = 0; i < configs; ++i)
            {
                benchmark->set_current_configuration(i);
                run_benchmark(benchmark);
            }
        }
        else
        {
            run_benchmark(benchmark);
        }
    }

    void runner::run_benchmark(benchmark_ptr benchmark)
    {
        assert(benchmark);
        assert(m_impl);

        if (m_impl->m_options.count("dry_run"))
        {
            return;
        }

        assert(!m_impl->m_current_benchmark);
        m_impl->m_current_benchmark = benchmark;

        benchmark->init();

        if (benchmark->needs_warmup_iteration())
        {
            benchmark->setup();
            benchmark->test_body();
            benchmark->tear_down();
        }

        uint32_t runs = 0;

        if (m_impl->m_options.count("runs"))
        {
            runs = m_impl->m_options["runs"].as<uint32_t>();
        }
        else
        {
            runs = benchmark->runs();
        }

        tables::table results;

        for (const auto &o : m_impl->m_columns)
        {
            results.add_const_column(o.first, o.second);
        }

        results.add_const_column("unit", benchmark->unit_text());
        results.add_const_column("benchmark", benchmark->benchmark_name());
        results.add_const_column("testcase", benchmark->testcase_name());

        for (auto& printer: enabled_printers())
        {
            printer->start_benchmark();
        }

        assert(runs > 0);
        uint32_t run = 0;

        results.add_column("iterations");
        results.add_column("run_number");

        while (run < runs)
        {
            benchmark->setup();
            benchmark->test_body();
            benchmark->tear_down();

            if (benchmark->accept_measurement())
            {
                results.add_row();
                results.set_value("iterations", benchmark->iteration_count());
                results.set_value("run_number", run);
                benchmark->store_run(results);
                ++run;
            }
        }

        // Clean out unwanted results
        if (m_impl->m_options.count("result_filter"))
        {
            auto f = m_impl->m_options["result_filter"].as<
                std::vector<std::string>>();

            for (auto& i : f)
            {
                if (!results.has_column(i))
                    continue;

                results.drop_column(i);
            }
        }

        // Notify all printers that we are done
        for (auto& printer: enabled_printers())
        {
            printer->end_benchmark();
        }

        for (auto& printer: enabled_printers())
        {
            printer->benchmark_result(*benchmark, results);
        }

        m_impl->m_current_benchmark = benchmark_ptr();
    }

    std::vector<runner::printer_ptr> runner::enabled_printers() const
    {
        std::vector<runner::printer_ptr> enabled_printers;

        for (auto& printer : m_impl->m_printers)
        {
            if (printer->is_enabled())
            {
                enabled_printers.push_back(printer);
            }
        }

        return enabled_printers;
    }

    std::vector<runner::printer_ptr>& runner::printers()
    {
        return m_impl->m_printers;
    }
}
