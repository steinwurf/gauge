// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gauge/gauge.hpp>
#include <cstdlib>

void max_array(double* x, double* y, double* z, uint32_t elements)
{
    for (uint32_t i = 0; i < elements; i++)
    {
        if (y[i] > x[i])
            z[i] = y[i];
        else
            z[i] = x[i];
    }
}

BENCHMARK(SimpleRun, MaxArray, 100)
{

    uint32_t elements = 10000;

    std::vector<double> x(elements);
    std::vector<double> y(elements);
    std::vector<double> z(elements);

    for (uint32_t i = 0; i < elements; ++i)
    {
        x[i] = rand();
        y[i] = rand();
    }

    // This is where the clock runs
    RUN
    {
        max_array(&x[0], &y[0], &z[0], elements);
    }

}

void max_array_opt(double* x, double* y, double* z, uint32_t elements)
{
    for (uint32_t i = 0; i < elements; i++)
    {
        z[i] = ((y[i] > x[i]) ? y[i] : x[i]);
    }
}

BENCHMARK(SimpleRun, MaxArrayOpt, 100)
{

    uint32_t elements = 10000;

    std::vector<double> x(elements);
    std::vector<double> y(elements);
    std::vector<double> z(elements);

    for (uint32_t i = 0; i < elements; ++i)
    {
        x[i] = rand();
        y[i] = rand();
    }

    RUN
    {
        max_array_opt(&x[0], &y[0], &z[0], elements);
    }
}
