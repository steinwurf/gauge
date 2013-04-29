#include <vector>
#include <algorithm>

#include "commandline_arguments.hpp"

namespace gauge
{

    commandline_arguements::commandline_arguements()
        : m_options("Options for gauge")
    {
        m_options.add_options()
            ("help", "produce help message")
            ("gauge_filter", po::value<std::string>(),
             "Filter which benchmarks to run based on their name "
              "for example ./benchmark --gauge_filter=MyTest.*")
            ("runs", po::value<uint32_t>(),
             "Sets the number of runs to complete. Overrides the "
             "settings specified in the benchmark ex. --runs=50");
    }

    po::variables_map
    commandline_arguements::parse(int argc, const char *argv[])
    {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, m_options), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << m_options << std::endl;
        }

        return vm;
    }

}
