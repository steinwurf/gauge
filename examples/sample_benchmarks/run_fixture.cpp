// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <list>

#include <gauge/gauge.hpp>

template<class Container>
class sort_data_setup : public gauge::time_benchmark
{
public:

    sort_data_setup()
        : m_elements(1000)
    {
        m_container.resize(m_elements, 0);
    }

    virtual void setup()
    {
        std::generate(std::begin(m_container), std::end(m_container), rand);
    }

    void test_body()
    {
        RUN
        {
            std::sort(std::begin(m_container), std::end(m_container));
        }
    }

protected:

    uint32_t m_elements;

    Container m_container;
};

//BENCHMARK_F_TEST(sort_data_setup<std::vector<uint32_t>>, SortVector, Uint32, 10)
//BENCHMARK_F_TEST(sort_data_setup<std::vector<uint8_t>>, SortVector, Uint8, 10)
