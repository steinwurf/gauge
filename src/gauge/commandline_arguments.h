#ifndef GAUGE_COMMANDLINE_ARGUMENTS_H
#define GAUGE_COMMANDLINE_ARGUMENTS_H

#include <boost/program_options.hpp>

namespace gauge
{

    namespace po = boost::program_options;

    /// Create a options map
    po::variables_map parse_commandline(int argc, const char *argv[]);
}

#endif
