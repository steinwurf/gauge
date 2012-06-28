#ifndef GAUGE_CONSOLE_PRINTER_H
#define GAUGE_CONSOLE_PRINTER_H

#include <iomanip>

#include "printer.h"
#include "statistics.h"
#include "console_colors.h"

namespace gauge
{

    /// A console printer which outputs the progress of the
    /// benchmarks to the console (std::cout)
    class console_printer : public printer
    {
    public: // From printer

        /// @see printer::start_benchmark
        void start_benchmark(/*const gauge_info &info*/)
        {
/*
            std::cout << std::fixed;
            std::cout << console::textGreen << "[==========]"
                      << console::textDefault << " Running "
                      << m_benchmarks
                      << (m_benchmarks == 1 ?
                          " benchmark." :
                          " benchmarks.")
                          << std::endl; */
        }



        void benchmark_result(const benchmark &info,
                              const result &result)
        {
            // Describe the beginning of the run.
            std::cout << std::fixed << console::textgreen << "[ RUN      ]"
                      << console::textdefault << " "
                      << info.testcase_name() << "."
                      << info.benchmark_name()
                      << " (" << result.m_results.size()
                      << (result.m_results.size() == 1 ? " run)" : " runs)")
                      << std::endl;

            statistics res = calculate_statistics(
                result.m_results.begin(),
                result.m_results.end());

            statistics iter = calculate_statistics(
                result.m_iterations.begin(),
                result.m_iterations.end());

            // Describe the end of the run.
            std::cout << console::textgreen << "[     DONE ]"
                      << console::textdefault << " "
                      << info.testcase_name() << "."
                      << info.benchmark_name() << " "
                      << "(" << iter.m_mean << " iterations per run)"
                      << std::endl;

            if(info.has_configurations())
            {
                std::cout << console::textyellow << "[  CONFIG  ]"
                          << console::textdefault << " "
                          << info.configuration_set() << std::endl;
            }

            std::cout << console::textyellow << "[   RUNS   ] "
                      << console::textdefault
                      << "       Average result: " << res.m_mean
                      << " " << info.unit_text() << std::endl;

            print("Max:", info.unit_text(), res.m_max, res.m_mean);
            print("Min:", info.unit_text(), res.m_min, res.m_mean);
        }

        void print(std::string name, std::string unit, double value, double mean)
        {
            std::cout << std::setw(35) << name << " " << value
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


