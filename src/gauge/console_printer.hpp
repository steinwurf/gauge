#ifndef GAUGE_CONSOLE_PRINTER_HPP
#define GAUGE_CONSOLE_PRINTER_HPP

#include <iomanip>

#include "printer.hpp"
#include "statistics.hpp"
#include "console_colors.hpp"

namespace gauge
{
    /// A console printer which outputs the progress of the
    /// benchmarks to the console (std::cout)
    class console_printer : public printer
    {
    public: // From printer

        void benchmark_result(const benchmark &info,
                              const table &results)
        {

            // Describe the beginning of the run.
            std::cout << std::fixed << console::textgreen << "[ RUN      ]"
                      << console::textdefault << " "
                      << info.testcase_name() << "."
                      << info.benchmark_name()
                      << " (" << results.rows()
                      << (results.rows() == 1 ? " run)" : " runs)")
                      << std::endl;

            if(info.has_configurations())
            {
                std::cout << console::textyellow << "[  CONFIG  ]"
                          << console::textdefault << " "
                          << info.get_current_configuration() << std::endl;
            }

            std::cout << console::textgreen << "[   RESULT ] "
                      << console::textdefault;

            std::vector<uint64_t> iterations =
                results.column_as<uint64_t>("iterations");

            statistics iter = calculate_statistics(
                iterations.cbegin(),
                iterations.cend());

            for(const auto& r : results)
            {
                if(!results.is_column<double>(r.first))
                    continue;

                std::vector<double> value =
                    results.column_as<double>(r.first);

                statistics res = calculate_statistics(
                    value.cbegin(),
                    value.cend());

                std::cout << r.first << " "
                          << "(" << iter.m_mean << " iterations per run):"
                          << std::endl
                          << console::textgreen << "[          ] "
                          << console::textdefault
                          << "       Average result: " << res.m_mean
                          << " " << info.unit_text() << std::endl;

                print("Max:", info.unit_text(), res.m_max, res.m_mean);
                print("Min:", info.unit_text(), res.m_min, res.m_mean);

            }

            std::cout << console::textgreen << "[----------] "
                      << console::textdefault << std::endl;
        }

        void print(std::string name, std::string unit,
                   double value, double mean)
        {
            std::cout << console::textgreen << "[          ] "
                      << console::textdefault
                      << std::setw(22) << name << " " << value
                      << " " << unit
                      << " ("
                      << (value < mean ? console::textred : console::textgreen)
                      << (value < mean ? "" : "+") << value - mean
                      << " " << unit << " / "
                      << (value < mean ? "" : "+")
                      << ((value-mean) * 100.0 / mean) << " %"
                      << console::textdefault << ")" << std::endl;
        }


        void end_benchmark(/*const gauge_info &info*/)
        {
/*            std::cout << console::textgreen << "[==========]"
                      << console::textdefault << " Ran "
                      << m_benchmarks
                      << (m_benchmarks == 1 ?
                          " benchmark." :
                          " benchmarks.")
                          << std::endl;*/
        }
};
}

#endif

