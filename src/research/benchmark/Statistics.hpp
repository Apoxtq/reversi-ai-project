/*
 * Statistics.hpp - Statistical Analysis Tools
 * COMP390 Honours Year Project - Week 10
 * 
 * Comprehensive statistical analysis for benchmark results
 * Calculates mean, standard deviation, confidence intervals, etc.
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cstdint>

namespace reversi {
namespace research {

/**
 * @brief Statistical analysis tools
 * 
 * Provides comprehensive statistical calculations:
 * - Mean, median, standard deviation
 * - 95% confidence intervals
 * - Min, max, quartiles
 * - Formatted output for academic reports
 */
class Statistics {
public:
    /**
     * @brief Statistical result structure
     */
    struct StatsResult {
        double mean = 0.0;              ///< Arithmetic mean
        double std_dev = 0.0;           ///< Standard deviation
        double median = 0.0;           ///< Median value
        double min = 0.0;               ///< Minimum value
        double max = 0.0;               ///< Maximum value
        double ci_95_lower = 0.0;       ///< 95% confidence interval lower bound
        double ci_95_upper = 0.0;       ///< 95% confidence interval upper bound
        int sample_size = 0;            ///< Number of samples
        
        /**
         * @brief Check if result is valid (has samples)
         */
        bool is_valid() const {
            return sample_size > 0;
        }
    };
    
    /**
     * @brief Calculate statistics from double vector
     * 
     * @param data Input data vector
     * @return Statistical result
     */
    static StatsResult calculate(const std::vector<double>& data);
    
    /**
     * @brief Calculate statistics from integer vector
     * 
     * @param data Input data vector
     * @return Statistical result
     */
    static StatsResult calculate(const std::vector<int>& data);
    
    /**
     * @brief Format as "mean ± std_dev" (academic format)
     * 
     * @param stats Statistical result
     * @param precision Number of decimal places
     * @return Formatted string
     */
    static std::string format_mean_std(
        const StatsResult& stats,
        int precision = 2
    );
    
    /**
     * @brief Format as "mean [CI_lower, CI_upper]" (confidence interval format)
     * 
     * @param stats Statistical result
     * @param precision Number of decimal places
     * @return Formatted string
     */
    static std::string format_mean_ci(
        const StatsResult& stats,
        int precision = 2
    );
    
    /**
     * @brief Format as "mean (std_dev)" (parentheses format)
     * 
     * @param stats Statistical result
     * @param precision Number of decimal places
     * @return Formatted string
     */
    static std::string format_mean_paren(
        const StatsResult& stats,
        int precision = 2
    );
    
    /**
     * @brief Calculate 95% confidence interval
     * 
     * Uses t-distribution for small samples (<30), normal for large samples
     * 
     * @param mean Sample mean
     * @param std_dev Sample standard deviation
     * @param sample_size Number of samples
     * @return Pair of (lower_bound, upper_bound)
     */
    static std::pair<double, double> calculate_confidence_interval(
        double mean,
        double std_dev,
        int sample_size
    );
    
    /**
     * @brief Calculate t-value for given confidence level and degrees of freedom
     * 
     * @param confidence_level Confidence level (0.95 for 95%)
     * @param degrees_of_freedom Degrees of freedom (sample_size - 1)
     * @return t-value
     * 
     * @note Uses approximation for t-distribution
     */
    static double t_value(double confidence_level, int degrees_of_freedom);
    
private:
    /**
     * @brief Calculate median from sorted data
     */
    static double calculate_median(const std::vector<double>& sorted_data);
    
    /**
     * @brief Calculate standard deviation
     */
    static double calculate_std_dev(
        const std::vector<double>& data,
        double mean
    );
};

} // namespace research
} // namespace reversi

