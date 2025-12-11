/*
 * minimax_vs_mcts.cpp - Minimax vs MCTS Comparison Experiment
 * COMP390 Honours Year Project - Week 10
 * 
 * Main experiment: Compare Minimax and MCTS algorithms
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "benchmark/Benchmark.hpp"
#include "benchmark/MatchEngine.hpp"
#include "benchmark/Statistics.hpp"
#include "ai/MinimaxEngine.hpp"
#include "ai/MCTSEngine.hpp"
#include <iostream>
#include <iomanip>

using namespace reversi::research;
using namespace reversi::ai;

void print_match_result(const MatchEngine::MatchResult& result) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Match Result: " << result.player1_name << " vs " << result.player2_name << "\n";
    std::cout << std::string(60, '=') << "\n";
    
    std::cout << "Total Games: " << result.total_games() << "\n";
    std::cout << "\nWin Statistics:\n";
    std::cout << "  " << result.player1_name << " wins: " << result.player1_wins 
              << " (" << std::fixed << std::setprecision(2) 
              << (result.player1_win_rate * 100.0) << "%)\n";
    std::cout << "  " << result.player2_name << " wins: " << result.player2_wins 
              << " (" << (result.player2_win_rate * 100.0) << "%)\n";
    std::cout << "  Draws: " << result.draws 
              << " (" << ((static_cast<double>(result.draws) / result.total_games()) * 100.0) << "%)\n";
    
    std::cout << "\nGame Statistics:\n";
    std::cout << "  Average game length: " << std::setprecision(1) 
              << result.avg_game_length << " moves\n";
    std::cout << "  Average duration: " << result.avg_duration_ms << " ms\n";
    
    // Calculate statistics for game lengths
    if (!result.game_lengths.empty()) {
        auto length_stats = Statistics::calculate(result.game_lengths);
        std::cout << "  Game length: " << Statistics::format_mean_std(length_stats, 1) << " moves\n";
    }
    
    std::cout << "\nPerformance Statistics:\n";
    std::cout << "  " << result.player1_name << ":\n";
    std::cout << "    Avg nodes/move: " << std::setprecision(0) 
              << result.avg_player1_nodes << "\n";
    std::cout << "    Avg time/move: " << std::setprecision(2) 
              << result.avg_player1_time_ms << " ms\n";
    
    std::cout << "  " << result.player2_name << ":\n";
    std::cout << "    Avg nodes/move: " << std::setprecision(0) 
              << result.avg_player2_nodes << "\n";
    std::cout << "    Avg time/move: " << std::setprecision(2) 
              << result.avg_player2_time_ms << " ms\n";
    
    std::cout << std::string(60, '=') << "\n";
}

int main(int argc, char* argv[]) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Minimax vs MCTS Comparison Experiment\n";
    std::cout << "COMP390 Honours Year Project - Week 10\n";
    std::cout << std::string(60, '=') << "\n\n";
    
    // Parse arguments
    int num_games = 20;
    if (argc > 1) {
        num_games = std::atoi(argv[1]);
    }
    
    bool alternate_colors = true;
    if (argc > 2) {
        alternate_colors = (std::atoi(argv[2]) != 0);
    }
    
    std::cout << "Configuration:\n";
    std::cout << "  Number of games: " << num_games << "\n";
    std::cout << "  Alternate colors: " << (alternate_colors ? "Yes" : "No") << "\n";
    std::cout << "\nStarting experiment...\n\n";
    
    // Configure search limits
    SearchLimits minimax_limits;
    minimax_limits.max_depth = 6;
    minimax_limits.max_time_ms = 5000;
    
    SearchLimits mcts_limits;
    mcts_limits.max_time_ms = 5000;
    // MCTS uses simulation count, not depth
    
    // Run comparison
    MatchEngine::MatchResult result = Benchmark::compare_minimax_vs_mcts(
        minimax_limits,
        mcts_limits,
        num_games,
        alternate_colors
    );
    
    // Print results
    print_match_result(result);
    
    // Export to CSV
    std::string csv_filename = "minimax_vs_mcts_results.csv";
    Benchmark::export_match_csv({result}, csv_filename);
    std::cout << "\nResults exported to: " << csv_filename << "\n";
    
    std::cout << "\nExperiment completed successfully!\n";
    return 0;
}

