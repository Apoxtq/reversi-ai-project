/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Benchmark Framework - Week 10 Enhanced
 */

#pragma once

#include "../../ai/AIStrategy.hpp"
#include "../../core/Board.hpp"
#include "MatchEngine.hpp"
#include "Statistics.hpp"
#include "PositionSuite.hpp"
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
    int avg_depth_reached = 0;
    
    // Performance breakdown
    std::vector<int> depth_distribution;
    std::vector<double> time_per_position;
    std::vector<uint64_t> nodes_per_position;
    
    // Statistical summary
    Statistics::StatsResult time_stats;
    Statistics::StatsResult nodes_stats;
    Statistics::StatsResult depth_stats;
};

/**
 * @brief Benchmark configuration
 */
struct BenchmarkConfig {
    int num_positions = 10;           ///< Number of test positions
    int repetitions = 1;              ///< Repetitions per position (for stochastic methods)
    PositionSuite::SuiteType suite_type = PositionSuite::SuiteType::STANDARD_64;
    bool use_opening_book = false;    ///< Use standard openings
    bool use_endgame_positions = true; ///< Include endgame tests
    uint32_t random_seed = 0;         ///< Random seed for reproducibility
    
    ai::SearchLimits search_limits;   ///< Search limits for AI
    
    std::string output_format = "csv"; ///< "console", "csv", "json"
    std::string output_file = "";      ///< Output filename
};

/**
 * @brief Benchmark framework for AI algorithm evaluation
 * 
 * Week 10 enhanced version with:
 * - Performance testing on position suites
 * - Match comparison between algorithms
 * - Statistical analysis
 * - CSV export
 */
class Benchmark {
public:
    /**
     * @brief Run benchmark on a single AI strategy
     * 
     * Tests performance on a suite of positions
     */
    static BenchmarkResult run_single(
        std::shared_ptr<ai::AIStrategy> strategy,
        const BenchmarkConfig& config
    );
    
    /**
     * @brief Compare multiple AI strategies
     * 
     * Runs benchmark on each strategy and returns results
     */
    static std::vector<BenchmarkResult> run_comparison(
        const std::vector<std::shared_ptr<ai::AIStrategy>>& strategies,
        const BenchmarkConfig& config
    );
    
    /**
     * @brief Run Minimax vs MCTS comparison
     * 
     * Convenience method for the main Week 10 experiment
     */
    static MatchEngine::MatchResult compare_minimax_vs_mcts(
        const ai::SearchLimits& minimax_limits,
        const ai::SearchLimits& mcts_limits,
        int num_games = 20,
        bool alternate_colors = true
    );
    
    /**
     * @brief Test different difficulty levels
     * 
     * Tests a single algorithm at different difficulty presets
     */
    static std::vector<BenchmarkResult> test_difficulty_levels(
        std::shared_ptr<ai::AIStrategy> strategy,
        const std::vector<ai::Difficulty>& levels,
        const BenchmarkConfig& base_config
    );
    
    /**
     * @brief Generate test positions
     * 
     * @deprecated Use PositionSuite::generate_suite() instead
     */
    static std::vector<core::Board> generate_test_positions(int count);
    
    /**
     * @brief Load standard test suite
     * 
     * @deprecated Use PositionSuite::generate_suite() instead
     */
    static std::vector<core::Board> load_standard_suite(const std::string& suite_name);
    
    /**
     * @brief Export benchmark results to CSV
     */
    static void export_results_csv(
        const std::vector<BenchmarkResult>& results,
        const std::string& filename
    );
    
    /**
     * @brief Export match results to CSV
     */
    static void export_match_csv(
        const std::vector<MatchEngine::MatchResult>& results,
        const std::string& filename
    );
    
    /**
     * @brief Export results to file (format auto-detected from extension)
     */
    static void export_results(
        const std::vector<BenchmarkResult>& results,
        const std::string& format,
        const std::string& filename
    );
};

} // namespace research
} // namespace reversi

