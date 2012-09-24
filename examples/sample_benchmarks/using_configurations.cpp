#include <algorithm>
#include <numeric>

#include <gauge/gauge.hpp>

/// This test shows how you can create configurations
/// for the benchmarks. This could be different string
/// lengths or similar
class using_configurations : public gauge::time_benchmark
{
public:

    using_configurations()
        {
            // Add some different configurations, in this case
            // we simply add them to the
            for(uint32_t k = 1; k < 4; ++k)
            {
                gauge::config_set cs;
                cs.set_value<uint32_t>("vector_length", 1000*k);
                add_configuration(cs);
            }

            gauge::config_set cs;
            cs.set_value<uint32_t>("vector_length", 1000000);
            add_configuration(cs);
        }

    void setup()
        {
            // Setup the test this could also be done inside the
            // BENCHMARK macro
            gauge::config_set cs = get_current_configuration();

            uint32_t length = cs.get_value<uint32_t>("vector_length");

            m_vector.resize(length);
            for(uint32_t i = 0; i < length; ++i)
                m_vector[i] = rand() % 5;
        }

    void tear_down()
        {
            // No implementation required
        }

    std::vector<uint32_t> m_vector;

};


BENCHMARK_F(using_configurations, Stdlib, Accumulate, 10)
{

    volatile uint32_t sum;
    RUN{
        sum = std::accumulate(m_vector.begin(), m_vector.end(), 0);
    }
    (void)sum; // Suppress warning about unused variable
}

