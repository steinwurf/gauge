#include "commandline_arguments.h"

#include <vector>
#include <algorithm>

namespace gauge
{
    namespace po = boost::program_options;

    po::variables_map parse_commandline(int argc, const char *argv[])
    {
        // Declare the supported options.
        po::options_description desc("Options for gauge");

        desc.add_options()
            ("help", "produce help message")
            ("gauge_filter", po::value<std::string>(),
             "Filter which benchmarks to run based on their name "
              "for example ./benchmark --gauge_filter=MyTest.*")
            ("runs", po::value<uint32_t>(),
             "Sets the number of runs to complete. Overrides the "
             "settings specified in the benchmark ex. --runs=50");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
        }

        return vm;
    }

}
