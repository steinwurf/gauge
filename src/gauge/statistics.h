#ifndef GAUGE_STATISTICS_H
#define GAUGE_STATISTICS_H

#include <algorithm>

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
        return std::accumulate(begin, end, 0.0) / size;
    }

    /// Calculate the statistics of a sequence
    template<class Iterator>
    statistics calculate_statistics(Iterator begin, Iterator end)
    {
        statistics s;
        s.m_mean = mean(begin, end);

        s.m_max = *std::max_element(begin, end);
        s.m_min = *std::min_element(begin, end);

        s.m_std_dev = 0;
        return s;
    }
}

#endif

