/*
 * Statistics.cpp - Statistical Analysis Tools Implementation
 * COMP390 Honours Year Project - Week 10
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "Statistics.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace reversi {
namespace research {

Statistics::StatsResult Statistics::calculate(const std::vector<double>& data) {
    StatsResult result;
    result.sample_size = static_cast<int>(data.size());
    
    if (result.sample_size == 0) {
        return result;
    }
    
    if (result.sample_size == 1) {
        result.mean = data[0];
        result.median = data[0];
        result.min = data[0];
        result.max = data[0];
        result.std_dev = 0.0;
        result.ci_95_lower = data[0];
        result.ci_95_upper = data[0];
        return result;
    }
    
    // Calculate mean
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    result.mean = sum / result.sample_size;
    
    // Calculate min and max
    auto minmax = std::minmax_element(data.begin(), data.end());
    result.min = *minmax.first;
    result.max = *minmax.second;
    
    // Calculate standard deviation
    result.std_dev = calculate_std_dev(data, result.mean);
    
    // Calculate median
    std::vector<double> sorted_data = data;
    std::sort(sorted_data.begin(), sorted_data.end());
    result.median = calculate_median(sorted_data);
    
    // Calculate 95% confidence interval
    auto ci = calculate_confidence_interval(result.mean, result.std_dev, result.sample_size);
    result.ci_95_lower = ci.first;
    result.ci_95_upper = ci.second;
    
    return result;
}

Statistics::StatsResult Statistics::calculate(const std::vector<int>& data) {
    // Convert to double vector
    std::vector<double> double_data(data.begin(), data.end());
    return calculate(double_data);
}

std::string Statistics::format_mean_std(
    const StatsResult& stats,
    int precision
) {
    if (!stats.is_valid()) {
        return "N/A";
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision)
        << stats.mean << " ± " << stats.std_dev;
    return oss.str();
}

std::string Statistics::format_mean_ci(
    const StatsResult& stats,
    int precision
) {
    if (!stats.is_valid()) {
        return "N/A";
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision)
        << stats.mean << " [" << stats.ci_95_lower << ", " << stats.ci_95_upper << "]";
    return oss.str();
}

std::string Statistics::format_mean_paren(
    const StatsResult& stats,
    int precision
) {
    if (!stats.is_valid()) {
        return "N/A";
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision)
        << stats.mean << " (" << stats.std_dev << ")";
    return oss.str();
}

std::pair<double, double> Statistics::calculate_confidence_interval(
    double mean,
    double std_dev,
    int sample_size
) {
    if (sample_size <= 1) {
        return {mean, mean};
    }
    
    // Calculate standard error
    double standard_error = std_dev / std::sqrt(static_cast<double>(sample_size));
    
    // Get t-value for 95% confidence (two-tailed)
    double t_val = t_value(0.95, sample_size - 1);
    
    // Calculate margin of error
    double margin = t_val * standard_error;
    
    return {mean - margin, mean + margin};
}

double Statistics::t_value(double confidence_level, int degrees_of_freedom) {
    // For large samples (df >= 30), use normal distribution approximation (z = 1.96 for 95%)
    if (degrees_of_freedom >= 30) {
        if (confidence_level == 0.95) {
            return 1.96;
        } else if (confidence_level == 0.99) {
            return 2.576;
        } else if (confidence_level == 0.90) {
            return 1.645;
        }
    }
    
    // For small samples, use t-distribution approximation
    // Simplified approximation: t ≈ z * (1 + z^2 / (4*df))
    double z_value = 1.96; // For 95% confidence
    if (confidence_level == 0.99) {
        z_value = 2.576;
    } else if (confidence_level == 0.90) {
        z_value = 1.645;
    }
    
    double correction = 1.0 + (z_value * z_value) / (4.0 * degrees_of_freedom);
    return z_value * correction;
}

double Statistics::calculate_median(const std::vector<double>& sorted_data) {
    size_t n = sorted_data.size();
    if (n == 0) {
        return 0.0;
    }
    
    if (n % 2 == 0) {
        // Even number of elements: average of two middle values
        return (sorted_data[n / 2 - 1] + sorted_data[n / 2]) / 2.0;
    } else {
        // Odd number of elements: middle value
        return sorted_data[n / 2];
    }
}

double Statistics::calculate_std_dev(
    const std::vector<double>& data,
    double mean
) {
    if (data.size() <= 1) {
        return 0.0;
    }
    
    double sum_squared_diff = 0.0;
    for (double value : data) {
        double diff = value - mean;
        sum_squared_diff += diff * diff;
    }
    
    // Sample standard deviation (divide by n-1)
    double variance = sum_squared_diff / (data.size() - 1);
    return std::sqrt(variance);
}

} // namespace research
} // namespace reversi

