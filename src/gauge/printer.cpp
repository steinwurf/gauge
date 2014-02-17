// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>
#include <tables/table.hpp>

#include "benchmark.hpp"
#include "file_printer.hpp"
#include "runner.hpp"

namespace gauge
{
    printer::printer(const std::string& name, bool enabled) :
        m_name(name),
        m_enabled(enabled)
    {
        gauge::po::options_description options;

        options.add_options()(
            ("use_" + m_name).c_str(),
            po::value<bool>()->default_value(m_enabled),
            ("Use the " + m_name + " printer").c_str());

        gauge::runner::instance().register_options(options);
    }

    bool printer::is_enabled() const
    {
        return m_enabled;
    }

    void printer::set_options(po::variables_map& options)
    {
        m_enabled = options["use_" + m_name].as<bool>();
    }
}
