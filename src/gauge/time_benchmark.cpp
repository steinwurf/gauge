#include "time_benchmark.h"
#include <boost/chrono.hpp>

namespace gauge
{
    namespace bc = boost::chrono;

    class time_benchmark::impl
    {
    public:

        /// The number of iterations to run to get an
        /// acceptable result
        uint32_t m_iterations;

        /// The threshold in microseconds before we accept
        /// a measurement
        double m_threshold;

        /// The start time
        bc::high_resolution_clock::time_point m_start;

        /// The stop time
        bc::high_resolution_clock::time_point m_stop;

        /// The result in microseconds
        double m_result;

    };


    /// How many micro seconds passes in one tick of the clock
    /// now mulitply that with a 100 then that is how long we should
    /// measure
    template <class Clock>
    double display_precision()
    {
        auto s = typename Clock::duration(1);
        return bc::duration_cast<bc::microseconds>(s).count() * 1000.0;
    }


    time_benchmark::time_benchmark()
        : m_impl(new time_benchmark::impl())
    {
    }

    time_benchmark::~time_benchmark()
    {}

    void time_benchmark::init()
    {
        m_impl->m_iterations = 1;
        m_impl->m_threshold = 10000.0;
    }

    uint32_t time_benchmark::iteration_count() const
    {
        return m_impl->m_iterations;
    }

    void time_benchmark::start()
    {
        m_impl->m_start = bc::high_resolution_clock::now();
    }

    void time_benchmark::stop()
    {
        m_impl->m_stop = bc::high_resolution_clock::now();

        m_impl->m_result = bc::duration_cast<bc::microseconds>(
            m_impl->m_stop-m_impl->m_start).count();
    }

    double time_benchmark::measurement()
    {
        return m_impl->m_result;
    }

    bool time_benchmark::accept_measurement()
    {
        assert(m_impl->m_threshold > 0);
        assert(m_impl->m_iterations > 0);

        if(m_impl->m_result >= m_impl->m_threshold)
        {
            return true;
        }

        if(m_impl->m_result == 0)
        {
            // We did not measure long enough to get a
            // valid time duration so we double the number of
            // iterations
            m_impl->m_iterations = m_impl->m_iterations * 2;
            return false;
        }

        // We did not get a valid measurement so we try to calculate
        // the number of iterations we need to hit the threshold
        double factor = m_impl->m_threshold / m_impl->m_result;
        assert(factor > 1.0);

        // Adjust the number of iterations with the factor
        m_impl->m_iterations = (m_impl->m_iterations * factor) + 1;
        assert(m_impl->m_iterations > 0);

        return false;
    }

    std::string time_benchmark::unit_text() const
    {
        return "microseconds";
    }

}




