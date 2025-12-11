/*
 * Benchmark.cpp - Benchmark Framework Implementation
 * COMP390 Honours Year Project - Week 10
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "Benchmark.hpp"
#include "../../ai/MinimaxEngine.hpp"
#include "../../ai/MCTSEngine.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <numeric>

namespace reversi {
namespace research {

BenchmarkResult Benchmark::run_single(
    std::shared_ptr<ai::AIStrategy> strategy,
    const BenchmarkConfig& config
) {
    BenchmarkResult result;
    result.algorithm_name = strategy->get_name();
    
    // Generate test positions
    std::vector<core::Board> positions = PositionSuite::generate_suite(
        config.suite_type,
        config.num_positions,
        config.random_seed
    );
    
    result.positions_tested = static_cast<int>(positions.size());
    
    // Test on each position
    for (const auto& position : positions) {
        for (int rep = 0; rep < config.repetitions; ++rep) {
            strategy->reset();
            
            auto start = std::chrono::high_resolution_clock::now();
            core::Move best_move = strategy->find_best_move(position, config.search_limits);
            auto end = std::chrono::high_resolution_clock::now();
            
            double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
            const auto& stats = strategy->get_stats();
            
            result.total_nodes += stats.nodes_searched;
            result.total_time_ms += static_cast<int>(time_ms);
            result.time_per_position.push_back(time_ms);
            result.nodes_per_position.push_back(stats.nodes_searched);
            result.depth_distribution.push_back(stats.depth_reached);
        }
    }
    
    // Calculate averages
    if (result.positions_tested > 0 && config.repetitions > 0) {
        int total_tests = result.positions_tested * config.repetitions;
        result.nodes_per_second = (result.total_nodes * 1000.0) / result.total_time_ms;
        
        if (!result.depth_distribution.empty()) {
            result.avg_depth_reached = std::accumulate(
                result.depth_distribution.begin(),
                result.depth_distribution.end(), 0
            ) / static_cast<int>(result.depth_distribution.size());
        }
        
        // Calculate statistics
        result.time_stats = Statistics::calculate(result.time_per_position);
        result.nodes_stats = Statistics::calculate(result.nodes_per_position);
        result.depth_stats = Statistics::calculate(result.depth_distribution);
    }
    
    return result;
}

std::vector<BenchmarkResult> Benchmark::run_comparison(
    const std::vector<std::shared_ptr<ai::AIStrategy>>& strategies,
    const BenchmarkConfig& config
) {
    std::vector<BenchmarkResult> results;
    
    for (const auto& strategy : strategies) {
        BenchmarkResult result = run_single(strategy, config);
        results.push_back(result);
    }
    
    return results;
}

MatchEngine::MatchResult Benchmark::compare_minimax_vs_mcts(
    const ai::SearchLimits& minimax_limits,
    const ai::SearchLimits& mcts_limits,
    int num_games,
    bool alternate_colors
) {
    // Create AI instances
    auto minimax = std::make_shared<ai::MinimaxEngine>();
    auto mcts = std::make_shared<ai::MCTSEngine>();
    
    // Configure match
    MatchEngine::MatchConfig match_config;
    match_config.num_games = num_games;
    match_config.alternate_colors = alternate_colors;
    match_config.verbose = true;
    
    // Play match
    MatchEngine::MatchResult result = MatchEngine::play_match(minimax, mcts, match_config);
    
    return result;
}

std::vector<BenchmarkResult> Benchmark::test_difficulty_levels(
    std::shared_ptr<ai::AIStrategy> strategy,
    const std::vector<ai::Difficulty>& levels,
    const BenchmarkConfig& base_config
) {
    std::vector<BenchmarkResult> results;
    
    for (ai::Difficulty level : levels) {
        BenchmarkConfig config = base_config;
        config.search_limits = ai::get_limits_from_difficulty(level);
        
        BenchmarkResult result = run_single(strategy, config);
        result.algorithm_name = strategy->get_name() + "_" + 
                                (level == ai::Difficulty::EASY ? "Easy" :
                                 level == ai::Difficulty::MEDIUM ? "Medium" : "Hard");
        results.push_back(result);
    }
    
    return results;
}

std::vector<core::Board> Benchmark::generate_test_positions(int count) {
    return PositionSuite::generate_suite(PositionSuite::SuiteType::STANDARD_64, count);
}

std::vector<core::Board> Benchmark::load_standard_suite(const std::string& suite_name) {
    // For now, just generate standard suite
    // File loading can be added later if needed
    return PositionSuite::generate_standard_64();
}

void Benchmark::export_results_csv(
    const std::vector<BenchmarkResult>& results,
    const std::string& filename
) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return;
    }
    
    // Write header
    file << "Algorithm,Positions,Repetitions,TotalNodes,TotalTimeMs,NodesPerSec,"
         << "AvgDepth,TimeMean,TimeStdDev,TimeCI95Lower,TimeCI95Upper,"
         << "NodesMean,NodesStdDev,NodesCI95Lower,NodesCI95Upper\n";
    
    // Write data
    for (const auto& result : results) {
        file << std::fixed << std::setprecision(2);
        file << result.algorithm_name << ","
             << result.positions_tested << ","
             << (result.positions_tested > 0 ? (result.time_per_position.size() / result.positions_tested) : 0) << ","
             << result.total_nodes << ","
             << result.total_time_ms << ","
             << result.nodes_per_second << ","
             << result.avg_depth_reached << ","
             << result.time_stats.mean << ","
             << result.time_stats.std_dev << ","
             << result.time_stats.ci_95_lower << ","
             << result.time_stats.ci_95_upper << ","
             << result.nodes_stats.mean << ","
             << result.nodes_stats.std_dev << ","
             << result.nodes_stats.ci_95_lower << ","
             << result.nodes_stats.ci_95_upper << "\n";
    }
    
    file.close();
}

void Benchmark::export_match_csv(
    const std::vector<MatchEngine::MatchResult>& results,
    const std::string& filename
) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return;
    }
    
    // Write header
    file << "Player1,Player2,TotalGames,Player1Wins,Player2Wins,Draws,"
         << "Player1WinRate,Player2WinRate,AvgGameLength,AvgDurationMs,"
         << "AvgPlayer1Nodes,AvgPlayer2Nodes,AvgPlayer1TimeMs,AvgPlayer2TimeMs\n";
    
    // Write data
    for (const auto& result : results) {
        file << std::fixed << std::setprecision(4);
        file << result.player1_name << ","
             << result.player2_name << ","
             << result.total_games() << ","
             << result.player1_wins << ","
             << result.player2_wins << ","
             << result.draws << ","
             << result.player1_win_rate << ","
             << result.player2_win_rate << ","
             << result.avg_game_length << ","
             << result.avg_duration_ms << ","
             << result.avg_player1_nodes << ","
             << result.avg_player2_nodes << ","
             << result.avg_player1_time_ms << ","
             << result.avg_player2_time_ms << "\n";
    }
    
    file.close();
}

void Benchmark::export_results(
    const std::vector<BenchmarkResult>& results,
    const std::string& format,
    const std::string& filename
) {
    if (format == "csv" || (format == "auto" && filename.find(".csv") != std::string::npos)) {
        export_results_csv(results, filename);
    } else {
        // Default to CSV
        export_results_csv(results, filename);
    }
}

} // namespace research
} // namespace reversi
