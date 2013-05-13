#pragma once

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

            std::vector<uint64_t> iterations =
                results.column_as<uint64_t>("iterations");

            statistics iter = calculate_statistics(
                iterations.cbegin(),
                iterations.cend());


            // Describe the beginning of the run.
            std::cout << std::fixed << console::textgreen << "[ RUN      ]"
                      << console::textdefault << " "
                      << info.testcase_name() << "."
                      << info.benchmark_name()
                      << " (" << results.rows()
                      << (results.rows() == 1 ? " run " : " runs ")
                      << "/ " << iter.m_mean << " "
                      << (iter.m_mean == 1 ? "iteration" : "iterations")
                      << ")" << std::endl;

            if(info.has_configurations())
            {
                std::cout << console::textyellow << "[  CONFIG  ]"
                          << console::textdefault << " "
                          << info.get_current_configuration() << std::endl;
            }

            for(const auto& r : results)
            {
                if(r.first == "iterations")
                    continue;

                if(print_column<double>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<float>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<uint64_t>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<int64_t>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<int32_t>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<uint32_t>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<int16_t>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<uint16_t>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<int8_t>(r.first,info.unit_text(),results))
                   continue;
                if(print_column<uint8_t>(r.first,info.unit_text(),results))
                   continue;

            }

            std::cout << console::textgreen << "[----------] "
                      << console::textdefault << std::endl;
        }

        template<class T>
        bool print_column(const std::string& column,
                          const std::string& unit,
                          const table& results)
        {
            if(!results.is_column<T>(column))
                return false;

            auto values = results.column_as<T>(column);

            statistics res = calculate_statistics(
                values.cbegin(),
                values.cend());

            std::cout << console::textgreen << "[   RESULT ] "
                      << console::textdefault;

            std::cout << column << " "
                      << std::endl
                      << console::textgreen << "[          ] "
                      << console::textdefault
                      << "   Average: " << res.m_mean
                      << " " << unit << std::endl;

            print("Max:", unit, res.m_max, res.m_mean);
            print("Min:", unit, res.m_min, res.m_mean);

            return true;

        }

        void print(std::string name, std::string unit,
                   double value, double mean)
        {
            std::cout << console::textgreen << "[          ] "
                      << console::textdefault
                      << std::setw(11) << name << " " << value
                      << " " << unit
                      << " ("
                      << (value < mean ? console::textred : console::textgreen)
                      << (value < mean ? "" : "+") << value - mean
                      << " " << unit << " / "
                      << (value < mean ? "" : "+")
                      << ((value-mean) * 100.0 / mean) << " %"
                      << console::textdefault << ")" << std::endl;
        }

    };
}


