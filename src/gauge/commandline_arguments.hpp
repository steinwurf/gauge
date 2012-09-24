#ifndef GAUGE_COMMANDLINE_ARGUMENTS_HPP
#define GAUGE_COMMANDLINE_ARGUMENTS_HPP

#include <boost/program_options.hpp>

namespace gauge
{
    namespace po = boost::program_options;

    /// Create a options map
    po::variables_map parse_commandline(int argc, const char *argv[]);
}

#endif
