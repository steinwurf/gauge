#include <gauge/gauge.h>
#include <gauge/console_printer.h>
#include <vector>

BENCHMARK(MyTest, RunThis, 100)
{
    std::vector<int> integers;
    for(int i = 0; i < 2048; ++i)
    {
        integers.push_back(i);
    }

    // This is where the clock runs
    RUN{
        for(int i = 1; i < integers.size(); ++i)
        {
            integers[i] += integers[i-1];
        }
    }
}

int main(int argc, const char* argv[])
{
    gauge::runner::instance().printers().push_back(
        std::make_shared<gauge::console_printer>());

    gauge::runner::run_benchmarks(argc, argv);
    return 0;
}
