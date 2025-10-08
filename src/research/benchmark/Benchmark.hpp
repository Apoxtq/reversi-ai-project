/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Benchmark Framework
 */

#pragma once

#include "../../ai/AIStrategy.hpp"
#include "../../core/Board.hpp"
#include <string>
#include <vector>
#include <memory>
#include <chrono>

namespace reversi {
namespace research {

/**
 * @brief Result of a single benchmark run
 */
struct BenchmarkResult {
    std::string algorithm_name;
    uint64_t total_nodes = 0;
    int total_time_ms = 0;
    double nodes_per_second = 0.0;
    int positions_tested = 0;
    
    // Performance breakdown
    std::vector<int> depth_distribution;
    std::vector<double> time_per_position;
};

/**
 * @brief Benchmark configuration
 */
struct BenchmarkConfig {
    int num_positions = 10;           // Number of test positions
    int repetitions = 3;              // Repetitions per position
    bool use_opening_book = false;    // Use standard openings
    bool use_endgame_positions = true; // Include endgame tests
    
    std::string output_format = "console"; // "console", "csv", "json"
    std::string output_file = "";
};

/**
 * @brief Benchmark framework for AI algorithm evaluation
 * 
 * This class will be implemented in Week 10-11 for final evaluations.
 * For now, it provides a minimal interface.
 */
class Benchmark {
public:
    /**
     * @brief Run benchmark on a single AI strategy
     */
    static BenchmarkResult run_single(
        const std::shared_ptr<ai::AIStrategy>& strategy,
        const BenchmarkConfig& config
    );
    
    /**
     * @brief Compare multiple AI strategies
     */
    static std::vector<BenchmarkResult> run_comparison(
        const std::vector<std::shared_ptr<ai::AIStrategy>>& strategies,
        const BenchmarkConfig& config
    );
    
    /**
     * @brief Generate test positions
     */
    static std::vector<core::Board> generate_test_positions(int count);
    
    /**
     * @brief Load standard test suite (Week 10)
     */
    static std::vector<core::Board> load_standard_suite(const std::string& suite_name);
    
    /**
     * @brief Export results to file
     */
    static void export_results(
        const std::vector<BenchmarkResult>& results,
        const std::string& format,
        const std::string& filename
    );
};

} // namespace research
} // namespace reversi

