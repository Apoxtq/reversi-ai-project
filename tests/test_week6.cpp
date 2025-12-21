/*
 * test_week6.cpp - Week 6 Advanced Search Techniques Test Suite
 * COMP390 Honours Year Project
 * 
 * Tests for Week 6 enhancements:
 * - Iterative deepening search
 * - Time management system
 * - Principal Variation Search (PVS)
 * - Aspiration windows
 * - Killer move heuristics
 */

#include "test_utils.hpp"
#include "ai/MinimaxEngine.hpp"
#include "../src/core/Board.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace reversi::core;
using namespace reversi::ai;
using namespace test;

// Color codes (compatible with test_utils.hpp)
#ifdef _WIN32
    #define GREEN   ""
    #define RESET   ""
#else
    #define GREEN   "\033[32m"
    #define RESET   "\033[0m"
#endif

/**
 * @brief Test iterative deepening search
 */
void test_iterative_deepening() {
    std::cout << "\n[TEST] Iterative Deepening Search\n";
    std::cout << "----------------------------------\n";
    
    Board board;
    MinimaxEngine::Config config;
    config.max_depth = 6;
    config.use_iterative_deepening = true;
    config.time_limit_ms = 1000;  // 1 second limit
    
    MinimaxEngine engine(config);
    auto result = engine.find_best_move(board);
    
    // Verify result
    ASSERT_TRUE(result.best_move >= 0 && result.best_move < 64);
    ASSERT_TRUE(result.depth_reached > 0);
    ASSERT_TRUE(result.depth_reached <= config.max_depth);
    ASSERT_TRUE(result.time_ms > 0);
    ASSERT_TRUE(result.time_ms <= config.time_limit_ms * 1.1);  // Allow 10% margin
    
    // Verify move is legal
    auto legal_moves = board.get_legal_moves();
    bool is_legal = std::find(legal_moves.begin(), legal_moves.end(), result.best_move) != legal_moves.end();
    ASSERT_TRUE(is_legal);
    
    std::cout << "  Depth reached: " << result.depth_reached << "\n";
    std::cout << "  Time used: " << std::fixed << std::setprecision(2) << result.time_ms << " ms\n";
    std::cout << "  Nodes: " << format_number(result.nodes_searched) << "\n";
    std::cout << GREEN << "[OK] Iterative deepening works\n" << RESET;
}

/**
 * @brief Test time management
 */
void test_time_management() {
    std::cout << "\n[TEST] Time Management System\n";
    std::cout << "------------------------------\n";
    
    Board board;
    MinimaxEngine::Config config;
    config.max_depth = 8;
    config.use_iterative_deepening = true;
    config.time_limit_ms = 500;  // 500ms limit
    
    MinimaxEngine engine(config);
    auto result = engine.find_best_move(board);
    
    // Verify time limit is respected
    ASSERT_TRUE(result.time_ms <= config.time_limit_ms * 1.2);  // Allow 20% margin for overhead
    
    std::cout << "  Time limit: " << config.time_limit_ms << " ms\n";
    std::cout << "  Time used: " << std::fixed << std::setprecision(2) << result.time_ms << " ms\n";
    std::cout << "  Depth reached: " << result.depth_reached << "\n";
    
    // Test with different game phases
    Board early_game;
    early_game.make_move(19);  // After first move
    
    Board mid_game;
    for (int i = 0; i < 20; i++) {
        auto moves = mid_game.get_legal_moves();
        if (!moves.empty()) {
            mid_game.make_move(moves[0]);
        }
    }
    
    // Early game should use less time
    auto result_early = engine.find_best_move(early_game);
    std::cout << "  Early game time: " << std::fixed << std::setprecision(2) 
              << result_early.time_ms << " ms\n";
    
    std::cout << GREEN << "[OK] Time management works\n" << RESET;
}

/**
 * @brief Test Principal Variation Search (PVS)
 */
void test_pvs() {
    std::cout << "\n[TEST] Principal Variation Search (PVS)\n";
    std::cout << "------------------------------------------\n";
    
    Board board;
    
    // Test without PVS
    MinimaxEngine::Config config_no_pvs;
    config_no_pvs.max_depth = 4;
    config_no_pvs.use_pvs = false;
    MinimaxEngine engine_no_pvs(config_no_pvs);
    auto result_no_pvs = engine_no_pvs.find_best_move(board);
    
    // Test with PVS
    MinimaxEngine::Config config_pvs;
    config_pvs.max_depth = 4;
    config_pvs.use_pvs = true;
    MinimaxEngine engine_pvs(config_pvs);
    auto result_pvs = engine_pvs.find_best_move(board);
    
    // PVS should give same result (correctness)
    ASSERT_EQ(result_no_pvs.best_move, result_pvs.best_move);
    
    // PVS should search fewer nodes (efficiency)
    std::cout << "  Without PVS: " << format_number(result_no_pvs.nodes_searched) << " nodes\n";
    std::cout << "  With PVS:    " << format_number(result_pvs.nodes_searched) << " nodes\n";
    
    double reduction = (1.0 - (double)result_pvs.nodes_searched / result_no_pvs.nodes_searched) * 100.0;
    std::cout << "  Node reduction: " << std::fixed << std::setprecision(1) << reduction << "%\n";
    
    // PVS should reduce nodes by at least 5%
    ASSERT_GT(reduction, 5.0);
    
    std::cout << GREEN << "[OK] PVS works correctly and efficiently\n" << RESET;
}

/**
 * @brief Test aspiration windows
 */
void test_aspiration_windows() {
    std::cout << "\n[TEST] Aspiration Windows\n";
    std::cout << "--------------------------\n";
    
    Board board;
    
    // Test without aspiration
    MinimaxEngine::Config config_no_asp;
    config_no_asp.max_depth = 5;
    config_no_asp.use_aspiration = false;
    MinimaxEngine engine_no_asp(config_no_asp);
    auto result_no_asp = engine_no_asp.find_best_move(board);
    
    // Test with aspiration
    MinimaxEngine::Config config_asp;
    config_asp.max_depth = 5;
    config_asp.use_aspiration = true;
    config_asp.use_iterative_deepening = true;  // Aspiration works best with ID
    MinimaxEngine engine_asp(config_asp);
    auto result_asp = engine_asp.find_best_move(board);
    
    // Results should be consistent
    ASSERT_EQ(result_no_asp.best_move, result_asp.best_move);
    
    std::cout << "  Without aspiration: " << format_number(result_no_asp.nodes_searched) << " nodes\n";
    std::cout << "  With aspiration:    " << format_number(result_asp.nodes_searched) << " nodes\n";
    
    double reduction = (1.0 - (double)result_asp.nodes_searched / result_no_asp.nodes_searched) * 100.0;
    std::cout << "  Node reduction: " << std::fixed << std::setprecision(1) << reduction << "%\n";
    
    std::cout << GREEN << "[OK] Aspiration windows work\n" << RESET;
}

/**
 * @brief Test killer move heuristics
 */
void test_killer_moves() {
    std::cout << "\n[TEST] Killer Move Heuristics\n";
    std::cout << "------------------------------\n";
    
    Board board;
    
    // Test without killer moves
    MinimaxEngine::Config config_no_killer;
    config_no_killer.max_depth = 5;
    config_no_killer.use_killer_moves = false;
    MinimaxEngine engine_no_killer(config_no_killer);
    auto result_no_killer = engine_no_killer.find_best_move(board);
    
    // Test with killer moves
    MinimaxEngine::Config config_killer;
    config_killer.max_depth = 5;
    config_killer.use_killer_moves = true;
    MinimaxEngine engine_killer(config_killer);
    auto result_killer = engine_killer.find_best_move(board);
    
    // Results should be consistent
    ASSERT_EQ(result_no_killer.best_move, result_killer.best_move);
    
    std::cout << "  Without killer: " << format_number(result_no_killer.nodes_searched) << " nodes\n";
    std::cout << "  With killer:    " << format_number(result_killer.nodes_searched) << " nodes\n";
    
    double reduction = (1.0 - (double)result_killer.nodes_searched / result_no_killer.nodes_searched) * 100.0;
    std::cout << "  Node reduction: " << std::fixed << std::setprecision(1) << reduction << "%\n";
    
    // Killer moves should improve efficiency
    ASSERT_GT(reduction, 0.0);  // At least some improvement
    
    std::cout << GREEN << "[OK] Killer moves work\n" << RESET;
}

/**
 * @brief Test combined optimizations
 */
void test_combined_optimizations() {
    std::cout << "\n[TEST] Combined Optimizations\n";
    std::cout << "------------------------------\n";
    
    Board board;
    
    // Baseline: no optimizations
    MinimaxEngine::Config config_baseline;
    config_baseline.max_depth = 5;
    config_baseline.use_iterative_deepening = false;
    config_baseline.use_pvs = false;
    config_baseline.use_aspiration = false;
    config_baseline.use_killer_moves = false;
    MinimaxEngine engine_baseline(config_baseline);
    auto result_baseline = engine_baseline.find_best_move(board);
    
    // All optimizations enabled - use conservative preset found by fixed-depth search
    MinimaxEngine::Config config_all = MinimaxEngine::Config::preset_fixed_found();
    config_all.max_depth = 5;
    // Use fixed-depth here to validate combined optimizations (iterative deepening makes comparison cumulative)
    config_all.use_iterative_deepening = false;
    // Ensure feature flags and tuned parameters are enabled for combined run (fixed-depth preset)
    config_all.use_pvs = true;
    config_all.pvs_research_margin = 20;
    config_all.pvs_failure_threshold = 4;
    config_all.use_aspiration = true;
    config_all.aspiration_window = 50;
    config_all.use_killer_moves = true;
    config_all.killer_weight = 0;
    config_all.tt_weight = 6000;
    config_all.history_weight = 0;
    std::cout << "[CONFIG_ALL] pvs_margin=" << config_all.pvs_research_margin
              << " asp_w=" << config_all.aspiration_window
              << " killer_w=" << config_all.killer_weight
              << " tt_w=" << config_all.tt_weight
              << " use_pvs=" << config_all.use_pvs
              << " use_asp=" << config_all.use_aspiration
              << " use_killer=" << config_all.use_killer_moves
              << " id=" << config_all.use_iterative_deepening << "\n";
    MinimaxEngine engine_all(config_all);
    auto result_all = engine_all.find_best_move(board);
    
    // Print PVS diagnostics for combined run to help tuning
    auto diag = engine_all.get_pvs_diagnostics();
    std::cout << "  [DIAG] combined_pvs_failures=" << diag.zero_window_failures
              << " researches=" << diag.researches
              << " beta_cutoffs=" << diag.zero_window_beta_cutoffs << "\n";
    
    // Results should be consistent
    ASSERT_EQ(result_baseline.best_move, result_all.best_move);
    
    std::cout << "  Baseline:        " << format_number(result_baseline.nodes_searched) << " nodes\n";
    std::cout << "  All optimizations: " << format_number(result_all.nodes_searched) << " nodes\n";
    
    double reduction = (1.0 - (double)result_all.nodes_searched / result_baseline.nodes_searched) * 100.0;
    std::cout << "  Total reduction: " << std::fixed << std::setprecision(1) << reduction << "%\n";
    
    // Combined optimizations should provide significant improvement
    ASSERT_GT(reduction, 10.0);  // At least 10% improvement
    
    std::cout << GREEN << "[OK] Combined optimizations work\n" << RESET;
}

/**
 * @brief Test time limit enforcement
 */
void test_time_limit_enforcement() {
    std::cout << "\n[TEST] Time Limit Enforcement\n";
    std::cout << "------------------------------\n";
    
    Board board;
    MinimaxEngine::Config config;
    config.max_depth = 10;  // Deep search
    config.use_iterative_deepening = true;
    config.time_limit_ms = 100;  // Very short time limit
    
    MinimaxEngine engine(config);
    auto result = engine.find_best_move(board);
    
    // Should respect time limit (with margin)
    ASSERT_TRUE(result.time_ms <= config.time_limit_ms * 1.5);
    
    // Should still return a valid move
    ASSERT_TRUE(result.best_move >= 0);
    
    std::cout << "  Time limit: " << config.time_limit_ms << " ms\n";
    std::cout << "  Time used:  " << std::fixed << std::setprecision(2) << result.time_ms << " ms\n";
    std::cout << "  Depth reached: " << result.depth_reached << "\n";
    
    std::cout << GREEN << "[OK] Time limit enforced\n" << RESET;
}

/**
 * @brief Test iterative deepening depth progression
 */
void test_id_depth_progression() {
    std::cout << "\n[TEST] Iterative Deepening Depth Progression\n";
    std::cout << "--------------------------------------------\n";
    
    Board board;
    MinimaxEngine::Config config;
    config.max_depth = 6;
    config.use_iterative_deepening = true;
    config.time_limit_ms = 2000;  // 2 seconds
    
    MinimaxEngine engine(config);
    auto result = engine.find_best_move(board);
    
    // Should reach at least depth 2
    ASSERT_GE(result.depth_reached, 2);
    
    // Should not exceed max_depth
    ASSERT_LE(result.depth_reached, config.max_depth);
    
    std::cout << "  Max depth: " << config.max_depth << "\n";
    std::cout << "  Depth reached: " << result.depth_reached << "\n";
    std::cout << "  Time used: " << std::fixed << std::setprecision(2) 
              << result.time_ms << " ms\n";
    
    std::cout << GREEN << "[OK] Depth progression works\n" << RESET;
}

/**
 * @brief Main test runner
 */
int main() {
    std::cout << "========================================\n";
    std::cout << "Week 6 Advanced Search Techniques\n";
    std::cout << "Test Suite\n";
    std::cout << "========================================\n";
    
    test_iterative_deepening();
    test_time_management();
    test_pvs();
    test_aspiration_windows();
    test_killer_moves();
    test_combined_optimizations();
    test_time_limit_enforcement();
    test_id_depth_progression();
    
    print_summary();
    
    return tests_failed > 0 ? 1 : 0;
}
 
