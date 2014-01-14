#include <ctime>

#include <gauge/gauge.hpp>

int main(int argc, const char* argv[])
{
    srand((uint32_t)time(0));

    gauge::runner::add_default_printers();
    gauge::runner::run_benchmarks(argc, argv);
    return 0;
}
