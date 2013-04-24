#pargma once

namespace gauge
{
    /// Struct containing the benchmark results from a single
    /// run.
    struct measurement
    {

        /// The result per iteration
        boost::any m_result;

        /// The number of iterations performed to obtain the
        /// corresponding result
        uint64_t m_iterations;

    };


    struct measurement_collector
    {



        /// The name of this result - describes the result type e.g.
        /// "time" or "throughput"
        std::string m_name;

        /// The unit of this result e.g. seconds or MB/s
        std::string m_unit;

        /// The collection of measurements
        std::vector<measurement> m_measurements;


    };

}
