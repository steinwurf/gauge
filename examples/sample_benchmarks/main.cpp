#include <ctime>

#include <gauge/gauge.hpp>
#include <gauge/python_printer.hpp>
#include <gauge/console_printer.hpp>
#include <gauge/csv_printer.hpp>


int main(int argc, const char* argv[])
{
    srand((uint32_t)time(0));

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::console_printer>());

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::python_printer>());

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::csv_printer>());

    gauge::runner::run_benchmarks(argc, argv);
    return 0;
}

