#include <gauge/gauge.h>

#include <gauge/python_printer.h>
#include <gauge/console_printer.h>


int main(int argc, const char* argv[])
{
   gauge::runner::instance().printers().push_back(
       std::make_shared<gauge::console_printer>());

    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::python_printer>("out.py"));

    gauge::runner::run_benchmarks(argc, argv);
    return 0;
}


