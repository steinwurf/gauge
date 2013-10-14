#pragma once

#include <iomanip>

#include <boost/chrono.hpp>
#include <tables/format.hpp>

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

        void start()
        {
            m_total_start = boost::chrono::high_resolution_clock::now();
        }

        void end()
        {
            m_total_stop = boost::chrono::high_resolution_clock::now();

            double time = static_cast<double>(
                boost::chrono::duration_cast<boost::chrono::microseconds>(
                    m_total_stop-m_total_start).count());

            std::cout << std::fixed << console::textgreen << "[     DONE ]"
                      << console::textdefault << " " << (time / 1000000)
                      << " seconds in total" << std::endl;
            std::cout << console::textgreen << "[----------] "
                      << console::textdefault << std::endl;

        }

        void start_benchmark()
        {
            m_benchmark_start = boost::chrono::high_resolution_clock::now();
        }

        void end_benchmark()
        {
            m_benchmark_stop = boost::chrono::high_resolution_clock::now();
        }


        void benchmark_result(const benchmark &info,
            const tables::table &results)
        {
            std::vector<uint64_t> iterations =
                results.values_as<uint64_t>("iterations");

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
                          << console::textdefault << " ";
                const auto& c = info.get_current_configuration();
                bool first = true;
                tables::format f;
                for(const auto& v : c)
                {
                    if(!first)
                        std::cout << ",";
                    first = false;
                    std::cout << v.first << "=";

                    f.print(std::cout, v.second);
                }

                std::cout << std::endl;
            }

            double time = static_cast<double>(
                boost::chrono::duration_cast<boost::chrono::microseconds>(
                    m_benchmark_stop-m_benchmark_start).count());

            std::cout << std::fixed << console::textyellow << "[   TIME   ]"
                      << console::textdefault << " " << (time / 1000)
                      << " milliseconds" << std::endl;


            for(const auto& c_name : results.columns())
            {
                if(c_name == "iterations")
                    continue;
                if(c_name == "run_number")
                    continue;
                if(print_column<double>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<float>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<uint64_t>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<int64_t>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<int32_t>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<uint32_t>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<int16_t>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<uint16_t>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<int8_t>(c_name,info.unit_text(),results))
                    continue;
                if(print_column<uint8_t>(c_name,info.unit_text(),results))
                    continue;

            }

            std::cout << console::textgreen << "[----------] "
                      << console::textdefault << std::endl;
        }

        template<class T>
        bool print_column(const std::string& column,
            const std::string& unit,
            const tables::table& results)
        {
            if(!results.is_column<T>(column))
                return false;

            if(results.empty_rows(column) > 0)
                return false;

            auto values = results.values_as<T>(column);

            statistics res = calculate_statistics(
                values.cbegin(),
                values.cend());

            std::cout << console::textgreen << "[   RESULT ] "
                      << console::textdefault;

            std::cout   << column << " "
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

    private:

        /// The start time
        boost::chrono::high_resolution_clock::time_point m_total_start;

        /// The stop time
        boost::chrono::high_resolution_clock::time_point m_total_stop;

        /// The start time
        boost::chrono::high_resolution_clock::time_point m_benchmark_start;

        /// The stop time
        boost::chrono::high_resolution_clock::time_point m_benchmark_stop;
    };
}
