#pragma once

#include <boost/program_options.hpp>

namespace gauge
{
    namespace po = boost::program_options;

    /// Create a options map
    //po::variables_map parse_commandline(int argc, const char *argv[]);

    class commandline_arguements
    {
    public:

        /// Constructor
        commandline_arguements();

        // void
        template<class T>
        void add_option(const char* option,
                        const char* description)
        {
            m_options.add_options()
                (option, po::value<T>(), description);
        }

        po::variables_map parse(int argc, const char *argv[]);

    private:

        /// The options
        po::options_description m_options;
    };

}

