#include <list>
#include <vector>

#include <gauge/gauge.hpp>

template<class Container>
uint32_t max_container(const Container &one)
{
    uint32_t max = 0;

    // RUN may be called outside the BENCHMARK macro as long a we
    // only call it once
    RUN{

        for (auto it = one.begin(); it != one.end(); it++)
        {
            if(*it > max)
                max = *it;
        }
    }

    return max;
}

template<typename Container>
uint32_t setup_and_run()
{
    uint32_t elements = 10000;
    Container c(elements);

    for(auto it = c.begin(); it != c.end(); ++it)
        *it = rand();

    return max_container(c);
}

BENCHMARK(RunOutside, VectorMaxValue, 10)
{
    uint32_t max = setup_and_run< std::vector<uint32_t> >();
    (void)max;
}

BENCHMARK(RunOutside, ListMaxValue, 10)
{
    uint32_t max = setup_and_run< std::list<uint32_t> >();
    (void)max;
}

