#include <cassert>

#include "runner.hpp"
#include "results.hpp"
#include "commandline_arguments.hpp"


namespace gauge
{
    struct runner::impl
    {
        /// Constructor
        impl() : m_current_id(0)
            {}

        /// The registered benchmarks
        std::map<uint32_t, benchmark_ptr> m_benchmarks;

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

        /// Id the the currently active benchmark
        uint32_t m_current_id;
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


    uint32_t runner::register_id()
    {
        // We start from one so we let 0 be an invalid benchmark id
        static uint32_t id = 1;

        // If zero is invalid we cannot have overflow, but that should
        // not be a problem - anyways better safe than sorry
        assert(id < std::numeric_limits<uint32_t>::max());
        return ++id;
    }

    void runner::add_benchmark(benchmark_ptr bench)
    {
        assert(bench);
        assert(m_impl);

        uint32_t id = bench->id();

        m_impl->m_benchmarks[id] = bench;

        std::string t_name = bench->testcase_name();
        std::string b_name = bench->benchmark_name();

        m_impl->m_testcases[t_name][b_name] = bench;

        bench->set_options(m_impl->m_options_description);
    }

    runner::benchmark_ptr runner::get_benchmark(uint32_t id)
    {
        assert(m_impl->m_benchmarks.find(id) != m_impl->m_benchmarks.end());
        return m_impl->m_benchmarks[id];
    }

    runner::benchmark_ptr runner::current_benchmark()
    {
        assert(m_impl->m_current_id != 0);
        return get_benchmark(m_impl->m_current_id);
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
        for(auto it = m_impl->m_benchmarks.begin();
            it != m_impl->m_benchmarks.end(); ++it)
        {
            benchmark_ptr bench = it->second;
            assert(bench);

            run_benchmark_configurations(bench);
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

        benchmark_map &benchmarks = m_impl->m_testcases[testcase_name];

        if(benchmark_name == "*")
        {
            for(auto it = benchmarks.begin(); it != benchmarks.end(); ++it)
            {
                run_benchmark_configurations(it->second);
            }
        }
        else
        {

            if(benchmarks.find(benchmark_name) == benchmarks.end())
            {
                throw std::runtime_error("Error benchmark not found");
            }

            benchmark_ptr bench = benchmarks.find(benchmark_name)->second;
            run_benchmark_configurations(bench);
        }

    }

    void runner::run_benchmark_configurations(benchmark_ptr bench)
    {
        assert(bench);
        assert(m_impl);

        bench->get_options(m_impl->m_options);

        if(bench->has_configurations())
        {
            uint32_t configs = bench->configuration_count();

            for(uint32_t i = 0; i < configs; ++i)
            {
                bench->set_current_configuration(i);
                run_benchmark(bench);
            }
        }
        else
        {
            run_benchmark(bench);
        }
    }

    void runner::run_benchmark(benchmark_ptr bench)
    {
        assert(bench);
        assert(m_impl);

        bench->init();

        assert(m_impl->m_current_id == 0);
        m_impl->m_current_id = bench->id();

        if(bench->needs_warmup_iteration())
        {
            bench->setup();
            bench->test_body();
            bench->tear_down();
        }

        results results;

        uint32_t runs = 0;

        if(m_impl->m_options.count("runs"))
        {
            runs = m_impl->m_options["runs"].as<uint32_t>();
        }
        else
        {
            runs = bench->runs();
        }

        assert(runs > 0);

        uint32_t run = 0;
        while(run < runs)
        {
            bench->setup();
            bench->test_body();
            bench->tear_down();

            if(bench->accept_measurement())
            {
                uint64_t i = bench->iteration_count();
                double r = bench->measurement();

                results.m_iterations.push_back(i);
                results.m_results.push_back(r);

                ++run;
            }
        }

        for(auto it = m_impl->m_printers.begin();
            it != m_impl->m_printers.end(); ++it)
        {
            (*it)->benchmark_result(*bench, results);
        }

        m_impl->m_current_id = 0;

    }


    std::vector<runner::printer_ptr>& runner::printers()
    {
        return m_impl->m_printers;
    }
}


