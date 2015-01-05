// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <list>

#include <gauge/gauge.hpp>

template<class Container>
class find_max_setup : public gauge::time_benchmark
{
public:

    find_max_setup()
    {
        m_container.resize(1000, 0);
    }

    virtual void setup()
    {
        // All containers will have the same random numbers
        srand(0);
        std::generate(std::begin(m_container), std::end(m_container), rand);
    }

    void test_body()
    {
        volatile uint32_t max_value;

        RUN
        {
            max_value = *std::max_element(std::begin(m_container),
                                          std::end(m_container));
        }

        (void) max_value;
    }

protected:

    Container m_container;
};

BENCHMARK_F(find_max_setup<std::vector<uint32_t>>, FindMax, FindInList, 10);
BENCHMARK_F(find_max_setup<std::list<uint32_t>>, FindMax, FindInVector, 10);
