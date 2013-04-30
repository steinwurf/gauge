#include <algorithm>
#include <numeric>

#include <gauge/gauge.hpp>

/// This test shows how you can create configurations
/// for the benchmarks. This could be different string
/// lengths or similar
class using_configurations : public gauge::time_benchmark
{
public:

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

    void set_options(gauge::po::options_description& options)
    {
        std::vector<int> v;
        v.push_back(1000);
        v.push_back(4000);

        auto default_option =
            gauge::po::value<std::vector<int> >()->default_value(
                v, "")->multitoken();

        options.add_options()
            ("vector_length", default_option, "set vector length");
    }

    void get_options(gauge::po::variables_map& options)
    {
        auto values = options["vector_length"].as<std::vector<int> >();
        for(auto v : values)
        {
            gauge::config_set cs;
            cs.set_value<uint32_t>("vector_length", v);
            add_configuration(cs);
        }
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

