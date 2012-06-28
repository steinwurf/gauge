#include "runner.h"
#include "commandline_arguments.h"
#include <boost/algorithm/string.hpp>

namespace gauge
{

    struct runner::impl
    {
        /// The registered benchmarks
        std::map<uint32_t, benchmark_ptr> m_benchmarks;

        /// Container for all the registered printers.
        std::vector<printer_ptr> m_printers;

        /// Test case map
        testcase_map m_testcases;

        /// Program options
        po::variables_map m_options;
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
        static uint32_t id = 0;
        return ++id;
    }

    void runner::add_benchmark(uint32_t id, benchmark_ptr bench)
    {
        m_impl->m_benchmarks[id] = bench;

        std::string t_name = bench->testcase_name();
        std::string b_name = bench->benchmark_name();

        m_impl->m_testcases[t_name][b_name] = bench;
    }

    runner::benchmark_ptr runner::get_benchmark(uint32_t id)
    {
        return m_impl->m_benchmarks[id];
    }

    void runner::run(int argc, const char *argv[])
    {

        try{
            m_impl->m_options = parse_commandline(argc, argv);
        }
        catch(const std::exception &e)
        {
            std::cout << "Error:" << e.what() << std::endl;
            return;
        }

        if(m_impl->m_options.count("help"))
            return;

        for(auto it = m_impl->m_printers.begin(); it != m_impl->m_printers.end(); ++it)
            (*it)->start_benchmark();

        if(m_impl->m_options.count("gauge_filter"))
        {
            run_filtered(m_impl->m_options["gauge_filter"].as<std::string>());
        }
        else
        {
            run_all();
        }

        for(auto it = m_impl->m_printers.begin(); it != m_impl->m_printers.end(); ++it)
            (*it)->end_benchmark();

    }


    void runner::run_all()
    {
        for(auto it = m_impl->m_benchmarks.begin(); it != m_impl->m_benchmarks.end(); ++it)
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
            throw std::runtime_error("Error malformed gauge_filter"
                                     " (example MyTest.*)");

        if(m_impl->m_testcases.find(testcase_name) == m_impl->m_testcases.end())
            throw std::runtime_error("Error testcase not found");

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
                throw std::runtime_error("Error benchmark not found");

            benchmark_ptr bench = benchmarks.find(benchmark_name)->second;
            run_benchmark_configurations(bench);
        }

    }

    void runner::run_benchmark_configurations(benchmark_ptr bench)
    {
        if(bench->has_configurations())
        {
            uint32_t configs = bench->configuration_count();

            for(uint32_t i = 0; i < configs; ++i)
            {
                bench->set_configuration(i);
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
        bench->init();

        if(bench->needs_warmup_iteration())
        {
            bench->setup();
            bench->test_body();
            bench->tear_down();
        }

        result result;

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
                double m = bench->measurement();
                uint32_t i = bench->iteration_count();
                double r = m/i;

                result.m_measurements.push_back(m);
                result.m_iterations.push_back(i);
                result.m_results.push_back(r);

                ++run;
            }
        }

        for(auto it = m_impl->m_printers.begin(); it != m_impl->m_printers.end(); ++it)
        {
            (*it)->benchmark_result(*bench, result);
        }

    }


    std::vector<runner::printer_ptr>& runner::printers()
    {
        return m_impl->m_printers;
    }

}



