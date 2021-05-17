// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <tables/format.hpp>
#include <tables/table.hpp>

#include <boost/program_options.hpp>

#include <map>
#include <string>
#include <vector>

#include "benchmark.hpp"
#include "printer.hpp"

namespace gauge
{

namespace po = boost::program_options;

/// A console printer which outputs the benchmark results based on the
/// provided formatter.
class stdout_printer : public printer
{
public:
    // from printer

    /// Create a new stdout printer
    stdout_printer();

    /// @see printer::set_options(po::variables_map&);
    void set_options(const po::variables_map& options);

    /// @see printer::benchmark_result(const benchmark&, const table&)
    void benchmark_result(const benchmark& info, const tables::table& results);

    /// @see printer::end()
    void end();

private:
    /// The type for the map containing each of the available formats
    typedef std::map<std::string, std::shared_ptr<tables::format>>
        formatter_map;

private:
    /// The key for the desired format
    formatter_map::key_type m_format_key;

    /// The available formatters
    formatter_map m_formatters;

    /// The output tables
    std::vector<tables::table> m_tables;
};
}
