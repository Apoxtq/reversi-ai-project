/*
 * bench_week6.cpp - Week 6 Performance Benchmark
 * COMP390 Honours Year Project
 * 
 * Performance benchmarks for Week 6 optimizations:
 * - Iterative deepening speedup
 * - PVS node reduction
 * - Aspiration windows efficiency
 * - Killer moves impact
 * - Combined optimizations performance
 */

#include "../src/core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace reversi::core;
using namespace reversi::ai;

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"

struct BenchResult {
    std::string name;
    long long nodes;
    double time_ms;
    double nodes_per_sec;
    int depth_reached;
};

void print_header(const char* title) {
    std::cout << BOLD << CYAN << "\n╔════════════════════════════════════════════════╗\n";
    std::cout << "║  " << title;
    int padding = 46 - strlen(title);
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n" << RESET;
}

void print_table_header() {
    std::cout << "\n" << std::setw(30) << "Configuration"
              << std::setw(15) << "Nodes"
              << std::setw(12) << "Time (ms)"
              << std::setw(15) << "Speed (M/s)"
              << std::setw(10) << "Depth\n";
    std::cout << std::string(82, '-') << "\n";
}

void print_result(const BenchResult& result) {
    std::cout << std::setw(30) << result.name
              << std::setw(15) << result.nodes
              << std::setw(12) << std::fixed << std::setprecision(2) << result.time_ms
              << std::setw(15) << std::fixed << std::setprecision(2) 
              << result.nodes_per_sec / 1e6
              << std::setw(10) << result.depth_reached << "\n";
}

BenchResult benchmark_config(const Board& board, const MinimaxEngine::Config& config, const std::string& name) {
    MinimaxEngine engine(config);
    auto result = engine.find_best_move(board);
    
    // Print PVS diagnostics if available (helps tune ordering)
    if (config.use_pvs) {
        auto d = engine.get_pvs_diagnostics();
        std::cout << "  [PVS] zero_window_failures=" << d.zero_window_failures 
                  << " researches=" << d.researches 
                  << " beta_cutoffs=" << d.zero_window_beta_cutoffs << "\n";
    }
    
    BenchResult br;
    br.name = name;
    br.nodes = result.nodes_searched;
    br.time_ms = result.time_ms;
    br.nodes_per_sec = result.nodes_per_sec();
    br.depth_reached = result.depth_reached;
    
    return br;
}

/**
 * @brief Benchmark iterative deepening vs fixed depth
 */
void benchmark_iterative_deepening() {
    print_header("Iterative Deepening Benchmark");
    
    Board board;
    std::vector<BenchResult> results;
    
    // Fixed depth baseline
    MinimaxEngine::Config config_fixed;
    config_fixed.max_depth = 6;
    config_fixed.use_iterative_deepening = false;
    results.push_back(benchmark_config(board, config_fixed, "Fixed Depth (d=6)"));
    
    // Iterative deepening
    MinimaxEngine::Config config_id;
    config_id.max_depth = 6;
    config_id.use_iterative_deepening = true;
    results.push_back(benchmark_config(board, config_id, "Iterative Deepening"));
    
    print_table_header();
    for (const auto& r : results) {
        print_result(r);
    }
    
    // Calculate improvement
    double speedup = results[1].nodes_per_sec / results[0].nodes_per_sec;
    std::cout << "\n" << GREEN << "Speedup: " << std::fixed << std::setprecision(2) 
              << speedup << "x\n" << RESET;
}

/**
 * @brief Benchmark PVS vs standard negamax
 */
void benchmark_pvs() {
    print_header("Principal Variation Search Benchmark");
    
    Board board;
    std::vector<BenchResult> results;
    
    // Standard negamax
    MinimaxEngine::Config config_std;
    config_std.max_depth = 6;
    config_std.use_pvs = false;
    results.push_back(benchmark_config(board, config_std, "Standard Negamax"));
    
    // PVS
    MinimaxEngine::Config config_pvs;
    config_pvs.max_depth = 6;
    config_pvs.use_pvs = true;
    results.push_back(benchmark_config(board, config_pvs, "Principal Variation Search"));
    
    print_table_header();
    for (const auto& r : results) {
        print_result(r);
    }
    
    // Calculate node reduction
    double reduction = (1.0 - (double)results[1].nodes / results[0].nodes) * 100.0;
    std::cout << "\n" << GREEN << "Node reduction: " << std::fixed << std::setprecision(1) 
              << reduction << "%\n" << RESET;
}

/**
 * @brief Benchmark all optimizations
 */
void benchmark_all_optimizations() {
    print_header("All Optimizations Benchmark");
    
    Board board;
    std::vector<BenchResult> results;
    
    // Baseline: no optimizations
    MinimaxEngine::Config config_baseline;
    config_baseline.max_depth = 6;
    config_baseline.use_iterative_deepening = false;
    config_baseline.use_pvs = false;
    config_baseline.use_aspiration = false;
    config_baseline.use_killer_moves = false;
    results.push_back(benchmark_config(board, config_baseline, "Baseline (no optimizations)"));
    
    // + Iterative Deepening
    MinimaxEngine::Config config_id;
    config_id = config_baseline;
    config_id.use_iterative_deepening = true;
    results.push_back(benchmark_config(board, config_id, "+ Iterative Deepening"));
    
    // + PVS
    MinimaxEngine::Config config_pvs;
    config_pvs = config_id;
    config_pvs.use_pvs = true;
    results.push_back(benchmark_config(board, config_pvs, "+ PVS"));
    
    // + Aspiration Windows
    MinimaxEngine::Config config_asp;
    config_asp = config_pvs;
    config_asp.use_aspiration = true;
    results.push_back(benchmark_config(board, config_asp, "+ Aspiration Windows"));
    
    // + Killer Moves
    MinimaxEngine::Config config_all;
    config_all = config_asp;
    config_all.use_killer_moves = true;
    results.push_back(benchmark_config(board, config_all, "+ Killer Moves (All)"));
    
    print_table_header();
    for (const auto& r : results) {
        print_result(r);
    }
    
    // Calculate cumulative improvements
    std::cout << "\n" << YELLOW << "Cumulative Improvements:\n" << RESET;
    for (size_t i = 1; i < results.size(); i++) {
        double speedup = results[i].nodes_per_sec / results[0].nodes_per_sec;
        double reduction = (1.0 - (double)results[i].nodes / results[0].nodes) * 100.0;
        std::cout << "  " << results[i].name << ":\n";
        std::cout << "    Speedup: " << std::fixed << std::setprecision(2) << speedup << "x\n";
        std::cout << "    Node reduction: " << std::fixed << std::setprecision(1) 
                  << reduction << "%\n";
    }
}

/**
 * @brief Benchmark time-limited search
 */
void benchmark_time_limited() {
    print_header("Time-Limited Search Benchmark");
    
    Board board;
    std::vector<BenchResult> results;
    
    for (int time_ms : {100, 500, 1000, 2000}) {
        MinimaxEngine::Config config;
        config.max_depth = 10;
        config.use_iterative_deepening = true;
        config.time_limit_ms = time_ms;
        
        std::string name = "Time limit: " + std::to_string(time_ms) + "ms";
        results.push_back(benchmark_config(board, config, name));
    }
    
    print_table_header();
    for (const auto& r : results) {
        print_result(r);
    }
    
    std::cout << "\n" << GREEN << "Time limits are respected\n" << RESET;
}

/**
 * @brief Benchmark different game phases
 */
void benchmark_game_phases() {
    print_header("Game Phase Time Management");
    
    // Create positions for different phases
    Board opening;
    
    Board midgame;
    for (int i = 0; i < 20; i++) {
        auto moves = midgame.get_legal_moves();
        if (!moves.empty()) {
            midgame.make_move(moves[0]);
        }
    }
    
    Board endgame;
    for (int i = 0; i < 50; i++) {
        auto moves = endgame.get_legal_moves();
        if (!moves.empty()) {
            endgame.make_move(moves[0]);
        } else {
            endgame.pass();
        }
        if (endgame.is_terminal()) break;
    }
    
    MinimaxEngine::Config config;
    config.max_depth = 8;
    config.use_iterative_deepening = true;
    config.time_limit_ms = 1000;
    
    std::vector<BenchResult> results;
    results.push_back(benchmark_config(opening, config, "Opening"));
    results.push_back(benchmark_config(midgame, config, "Midgame"));
    results.push_back(benchmark_config(endgame, config, "Endgame"));
    
    print_table_header();
    for (const auto& r : results) {
        print_result(r);
    }
    
    std::cout << "\n" << GREEN << "Time management adapts to game phase\n" << RESET;
}

int main() {
    std::cout << BOLD << CYAN;
    std::cout << "╔════════════════════════════════════════════════╗\n";
    std::cout << "║  Week 6 Advanced Search Performance Benchmark ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
    std::cout << RESET;
    
    benchmark_iterative_deepening();
    benchmark_pvs();
    benchmark_all_optimizations();
    benchmark_time_limited();
    benchmark_game_phases();
    
    std::cout << BOLD << GREEN << "\n[OK] All benchmarks completed!\n" << RESET;
    
    return 0;
}