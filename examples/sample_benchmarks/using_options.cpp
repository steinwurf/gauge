#include <algorithm>
#include <numeric>

#include <gauge/gauge.hpp>

/// This test shows how you can add options to the benchmark
/// options
class using_options : public gauge::time_benchmark
{
public:

    void setup()
    {
        // Setup the test this could also be done inside the
        // BENCHMARK macro
        gauge::config_set cs = get_current_configuration();

        uint32_t size = cs.get_value<uint32_t>("array_size");

        m_vector.resize(size);
        for(uint32_t i = 0; i < size; ++i)
            m_vector[i] = rand() % 256;
    }

    void tear_down()
    {
        // No implementation required
    }

    void set_options(gauge::po::options_description& options)
    {
        // Optionally check if the options has already been added,
        // if this is the case possible solutions are:
        // 1) Skip adding that option, and reuse the existing. This
        //    may be acceptable in cases where e.g. the same test fixture
        //    is reused between tests.
        // 2) Rename one of the options e.g. using the benchmark name.
        //
        // Since we reuse this test fixture we will skip adding the option
        // if it already exists
        if(options.find_nothrow("array_size", false) != 0)
            return;

        std::vector<int> size;
        size.push_back(10);
        size.push_back(40);

        auto default_size =
            gauge::po::value<std::vector<int> >()->default_value(
                size, "")->multitoken();

        options.add_options()
            ("array_size", default_size, "Set bit array size");
    }

    void get_options(gauge::po::variables_map& options)
    {
        auto values = options["array_size"].as<std::vector<int> >();
        for(auto v : values)
        {
            gauge::config_set cs;
            cs.set_value<uint32_t>("array_size", v);
            add_configuration(cs);
        }
    }

    std::vector<uint32_t> m_vector;

};

// Counting bits set, Brian Kernighan's way
uint32_t count_bk(uint32_t v)
{
    uint32_t c; // c accumulates the total bits set in v

    for (c = 0; v; c++)
    {
        v &= v - 1; // clear the least significant bit set
    }
    return c;
}


// Counting bits set (naive way)
uint32_t count_naive(uint32_t v)
{
    uint32_t c; // c accumulates the total bits set in v

    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    return c;
}

BENCHMARK_F(using_options, CountBits, count_bk, 10)
{

    volatile uint32_t sum;
    RUN{
        for(auto& v: m_vector)
            sum = count_bk(v);
    }
    (void)sum; // Suppress warning about unused variable
}

BENCHMARK_F(using_options, CountBits, count_naive, 10)
{

    volatile uint32_t sum;
    RUN{
        for(auto& v: m_vector)
            sum = count_naive(v);
    }
    (void)sum; // Suppress warning about unused variable
}


