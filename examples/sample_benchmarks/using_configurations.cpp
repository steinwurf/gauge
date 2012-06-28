#include "gauge.h"

#include "python_printer.h"
#include "console_printer.h"

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
                m_test_configs.push_back(cs);
            }

            gauge::config_set cs;
            cs.set_value<uint32_t>("vector_length", 1000000);
            m_test_configs.push_back(cs);
        }

    uint32_t configuration_count() const
        {
            // tell gauge how many configurations we have
            return m_test_configs.size();
        }

    void setup()
        {
            // Setup the test this could also be done inside the
            // BENCHMARK macro
            gauge::config_set cs = configuration_set();

            uint32_t length = cs.get_value<uint32_t>("vector_length");

            m_vector.resize(length);
            for(uint32_t i = 0; i < length; ++i)
                m_vector[i] = rand() % 5;
        }

    gauge::config_set configuration_set() const
        {
            // Implementing this function makes sure that gauge
            // can output information about the current configuration.
            // gauge tells us which configuration we should use
            uint32_t index = current_configuration();

            assert(index < m_test_configs.size());
            return m_test_configs[index];
        }

    void tear_down()
        {
            // No implementation required
        }

    std::vector<uint32_t> m_vector;
    std::vector<gauge::config_set> m_test_configs;

};


BENCHMARK_F(using_configurations, accumulate, 10)
{

    volatile uint32_t sum;
    RUN{
        sum = std::accumulate(m_vector.begin(), m_vector.end(), 0);
    }
    (void)sum; // Suppress warning about unused variable
}




