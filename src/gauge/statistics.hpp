#ifndef GAUGE_STATISTICS_HPP
#define GAUGE_STATISTICS_HPP

#include <algorithm>
#include <numeric>

namespace gauge
{
    /// The computed statistics for a specific
    /// result
    struct statistics
    {
        /// The mean value
        double m_mean;

        /// The maximum value observed
        double m_max;

        /// The minimum value observed
        double m_min;

        /// The standard deviation
        double m_std_dev;
    };

    /// Calculates the mean of a sequence
    template<class Iterator>
    double mean(Iterator begin, Iterator end)
    {
        uint32_t size = std::distance(begin, end);
        assert(size > 0);

        return std::accumulate(begin, end, 0.0) / size;
    }

    /// Calculate the statistics of a sequence
    template<class Iterator>
    statistics calculate_statistics(Iterator begin, Iterator end)
    {
        statistics s;
        s.m_mean = mean(begin, end);

        typename Iterator::value_type max =
            *std::max_element(begin, end);

        typename Iterator::value_type min =
            *std::min_element(begin, end);

        // @todo do we have an issues with overflows here
        s.m_max = static_cast<double>(max);
        s.m_min = static_cast<double>(min);

        s.m_std_dev = 0;
        return s;
    }
}

#endif

