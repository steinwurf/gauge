#include <boost/chrono.hpp>

#include "time_benchmark.hpp"

namespace gauge
{
    namespace bc = boost::chrono;

    class time_benchmark::impl
    {
    public:

        /// The number of iterations to run to get an
        /// acceptable result
        uint64_t m_iterations;

        /// The threshold in microseconds before we accept
        /// a measurement
        double m_threshold;

        /// The start time
        bc::high_resolution_clock::time_point m_start;

        /// The stop time
        bc::high_resolution_clock::time_point m_stop;

        /// The result in microseconds
        double m_result;

        /// Got result i.e. was start() and stop() called
        /// If the user forgets to use the RUN macro inside the
        /// BENCHMARK macro this test would trigger
        bool m_started;
        bool m_stopped;

        /// Stores whether the measurement was accepted
        bool m_accepted;

        /// Final result
        results m_results;

    };


    /// How many micro seconds passes in one tick of the clock
    /// now mulitply that with a 100 then that is how long we should
    /// measure
    //template <class Clock>
    //double display_precision()
    //{
    //    auto s = typename Clock::duration(1);
    //    return bc::duration_cast<bc::microseconds>(s).count() * 1000.0;
    //}


    time_benchmark::time_benchmark()
        : m_impl(new time_benchmark::impl())
    { }

    time_benchmark::~time_benchmark()
    { }

    void time_benchmark::init()
    {
        m_impl->m_iterations = 1;
        m_impl->m_threshold = 10000.0;
        m_impl->m_started = false;
        m_impl->m_stopped = false;
    }

    uint64_t time_benchmark::iteration_count() const
    {
        return m_impl->m_iterations;
    }

    void time_benchmark::start()
    {
        assert(m_impl->m_iterations > 0);
        m_impl->m_started = true;
        m_impl->m_start = bc::high_resolution_clock::now();
    }

    void time_benchmark::stop()
    {
        m_impl->m_stop = bc::high_resolution_clock::now();

        assert(m_impl->m_started);
        m_impl->m_stopped = true;

        m_impl->m_result = static_cast<double>(
            bc::duration_cast<bc::microseconds>(
                m_impl->m_stop-m_impl->m_start).count());

        assert(m_impl->m_iterations > 0);

        store_measurement();
    }

    double time_benchmark::measurement()
    {
        assert(m_impl);
        assert(m_impl->m_started);
        assert(m_impl->m_stopped);
        assert(m_impl->m_iterations > 0);

        return m_impl->m_result / m_impl->m_iterations;
    }

    bool time_benchmark::accept_measurement()
    {
        // Did you forget the RUN macro?
        assert(m_impl);
        assert(m_impl->m_started);
        assert(m_impl->m_stopped);
        assert(m_impl->m_threshold > 0);
        assert(m_impl->m_iterations > 0);

        return m_impl->m_accepted;
    }

    void time_benchmark::store_measurement()
    {
        // Did you forget the RUN macro?
        assert(m_impl);
        assert(m_impl->m_started);
        assert(m_impl->m_stopped);
        assert(m_impl->m_threshold > 0);
        assert(m_impl->m_iterations > 0);

        if(m_impl->m_result >= m_impl->m_threshold)
        {
            double factor = m_impl->m_result / m_impl->m_threshold;
            if(factor > 2.0)
            {
                // We seem to be running longer than needed
                m_impl->m_iterations = static_cast<uint32_t>(
                    (m_impl->m_iterations / factor) + 1);

                assert(m_impl->m_iterations > 0);
            }

            // m_impl->m_results.m_results.push_back(

            m_impl->m_accepted = true;
            return;
        }

        if(m_impl->m_result == 0)
        {
            // Check for overflow - are you sure you actually measure
            // anything - it seems time is zero even with a very large
            // number of iterations?
            assert((m_impl->m_iterations * 2) > m_impl->m_iterations);

            // We did not measure long enough to get a
            // valid time duration so we double the number of
            // iterations
            m_impl->m_iterations = m_impl->m_iterations * 2;
            assert(m_impl->m_iterations > 0);
            m_impl->m_accepted = false;
            return;
        }


        // We did not get a valid measurement so we try to calculate
        // the number of iterations we need to hit the threshold
        double factor = m_impl->m_threshold / m_impl->m_result;

        // We limit the growth of iteration by a factor of 10
        factor = std::min(factor, 10.0);

        assert(factor > 1.0);

        // Adjust the number of iterations with the factor
        m_impl->m_iterations = static_cast<uint32_t>(
            (m_impl->m_iterations * factor) + 1);

        assert(m_impl->m_iterations > 0);

        m_impl->m_accepted = false;

    }


    std::string time_benchmark::unit_text() const
    {
        return "microseconds";
    }

    void time_benchmark::store_results(temp_results &results)
    {
        auto& r = results["time"];
        if(r.m_unit.empty())
            r.m_unit = "microseconds";

        assert(results.find("time") != results.end());

        r.m_results.push_back(measurement());
        r.m_iterations.push_back(iteration_count());

        assert(r.m_results.size() == r.m_iterations.size());
    }


}

