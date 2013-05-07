#include <cassert>

#include "runner.hpp"
#include "results.hpp"
#include "commandline_arguments.hpp"

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

        /// Command-line arguments
        commandline_arguements m_commandline;

        /// The available program options
        po::options_description m_options_description;

        /// Parsed program options
        po::variables_map m_options;

        /// The currently active benchmark
        benchmark_ptr m_current_benchmark;
    };

    runner::runner()
        : m_impl(new runner::impl())
    {
        //m_printers.push_back(std::make_shared<console_printer>());
    }

    runner::~runner()
    {
    }

    runner& runner::instance()
    {
        static runner singleton;
        return singleton;
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
        for(const auto& o: opt)
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
        assert(m_impl);

        po::options_description options("Gauge");

        options.add_options()
            ("help", "produce help message")
            ("gauge_filter", po::value<std::string>(),
             "Filter which benchmarks to run based on their name "
              "for example ./benchmark --gauge_filter=MyTest.*")
            ("runs", po::value<uint32_t>(),
             "Sets the number of runs to complete. Overrides the "
             "settings specified in the benchmark ex. --runs=50");

        options.add(m_impl->m_options_description);

        try
        {
            po::variables_map vm;
            po::store(po::parse_command_line(argc, argv, options), vm);
            po::notify(vm);

            m_impl->m_options = vm;
        }
        catch(const std::exception &e)
        {
            std::cout << "Error:" << e.what() << std::endl;
            return;
        }

        if(m_impl->m_options.count("help"))
        {
            std::cout << options << std::endl;
            return;
        }

        // Deliver possible options to printers
        for(auto& p: m_impl->m_printers)
        {
            p->set_options(m_impl->m_options);
        }

        // Notify all printers that we are starting
        for(auto it = m_impl->m_printers.begin();
            it != m_impl->m_printers.end(); ++it)
        {
            (*it)->start_benchmark();
        }
        // Check whether we should run all tests or whether we
        // should use a filter
        if(m_impl->m_options.count("gauge_filter"))
        {
            auto f = m_impl->m_options["gauge_filter"].as<std::string>();
            run_filtered(f);
        }
        else
        {
            run_all();
        }

        // Notify all printers that we are done
        for(auto it = m_impl->m_printers.begin();
            it != m_impl->m_printers.end(); ++it)
        {
            (*it)->end_benchmark();
        }

    }

    void runner::run_all()
    {
        assert(m_impl);

        for(auto& m : m_impl->m_benchmarks)
        {
            auto& make = m.second;
            auto benchmark = make();

            assert(benchmark);

            run_benchmark_configurations(benchmark);
        }
    }

    void runner::run_filtered(const std::string &filter)
    {
        std::istringstream sstream(filter);

        std::string testcase_name;
        std::string benchmark_name;

        std::getline(sstream, testcase_name, '.');

        if(!sstream)
            throw std::runtime_error("Error malformed gauge_filter"
                                     " (example MyTest.*)");

        std::getline(sstream, benchmark_name);

        if(!sstream)
        {
            throw std::runtime_error("Error malformed gauge_filter"
                                     " (example MyTest.*)");
        }

        if(m_impl->m_testcases.find(testcase_name) ==
           m_impl->m_testcases.end())
        {
            throw std::runtime_error("Error testcase not found");
        }

        auto &benchmarks = m_impl->m_testcases[testcase_name];

        if(benchmark_name == "*")
        {
            for(auto& b : benchmarks)
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

            if(benchmarks.find(benchmark_name) == benchmarks.end())
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

        if(benchmark->has_configurations())
        {
            uint32_t configs = benchmark->configuration_count();

            for(uint32_t i = 0; i < configs; ++i)
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

        assert(!m_impl->m_current_benchmark);
        m_impl->m_current_benchmark = benchmark;

        benchmark->init();

        if(benchmark->needs_warmup_iteration())
        {
            benchmark->setup();
            benchmark->test_body();
            benchmark->tear_down();
        }

        uint32_t runs = 0;

        if(m_impl->m_options.count("runs"))
        {
            runs = m_impl->m_options["runs"].as<uint32_t>();
        }
        else
        {
            runs = benchmark->runs();
        }

        table results;
        results.set_unit(benchmark->unit_text());

        assert(runs > 0);
        uint32_t run = 0;

        while(run < runs)
        {
            benchmark->setup();
            benchmark->test_body();
            benchmark->tear_down();

            if(benchmark->accept_measurement())
            {
                results.add_run(benchmark->iteration_count());
                benchmark->store_run(results);
                ++run;
            }
        }

        assert(results.runs() == run);

        for(auto& printer : m_impl->m_printers)
        {
            printer->benchmark_result(*benchmark, results);
        }

        m_impl->m_current_benchmark = benchmark_ptr();

    }

    std::vector<runner::printer_ptr>& runner::printers()
    {
        return m_impl->m_printers;
    }
}


