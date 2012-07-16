#include <gauge/gauge.h>

#include <algorithm>
#include <list>

class find_max_setup : public gauge::time_benchmark
{
public:
    find_max_setup()
        : m_elements(1000)
        {
            m_vector.resize(m_elements, 0);
            m_list.resize(m_elements, 0);
        }

    virtual void setup()
        {
            // Setup a new vector and list for every run

            auto vector_it = m_vector.begin();
            auto list_it = m_list.begin();

            for(uint32_t i = 0; i < m_elements; ++i)
            {
                uint32_t r = rand();

                *vector_it = r;
                *list_it = r;

                ++vector_it;
                ++list_it;
            }
        }

    virtual void tear_down()
        {
            // Not needed in this case
        }

protected:

    uint32_t m_elements;

    std::vector<uint32_t> m_vector;
    std::list<uint32_t> m_list;


};


BENCHMARK_F(find_max_setup, FindMax, FindInList, 10)
{
    volatile uint32_t max_value;

    RUN{
        max_value = *std::max_element(m_list.begin(), m_list.end());
    }

    (void)max_value; // Suppress unused variable warning
}


BENCHMARK_F(find_max_setup, FindMax, FindInVector, 10)
{
    volatile uint32_t max_value;

    RUN{
        max_value = *std::max_element(m_vector.begin(), m_vector.end());
    }

    (void)max_value; // Suppress unused variable warning
}




