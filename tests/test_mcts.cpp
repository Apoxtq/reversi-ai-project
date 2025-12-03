/*
 * test_mcts.cpp - Unit tests for MCTS engine
 * COMP390 Honours Year Project
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "ai/MCTSEngine.hpp"
#include "core/Board.hpp"
#include "core/Move.hpp"
#include "test_utils.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace reversi;
using namespace reversi::ai;
using namespace reversi::core;

void test_mcts_basic_search() {
    std::cout << "Test: Basic MCTS search\n";
    MCTSEngine engine;
    Board board;
    SearchLimits limits(0, 1000); // 1 second
    
    Move move = engine.find_best_move(board, limits);
    assert(move.is_valid() || move.is_pass());
    
    const auto& stats = engine.get_mcts_stats();
    assert(stats.simulations_performed > 0);
    assert(stats.tree_nodes_created > 0);
    
    std::cout << "  ✓ Basic search passed\n";
    std::cout << "    Simulations: " << stats.simulations_performed << "\n";
    std::cout << "    Tree nodes: " << stats.tree_nodes_created << "\n";
}

void test_mcts_tree_growth() {
    std::cout << "Test: Tree growth\n";
    MCTSEngine engine;
    Board board;
    SearchLimits limits(0, 2000); // 2 seconds
    
    Move move = engine.find_best_move(board, limits);
    const auto& stats = engine.get_mcts_stats();
    
    assert(stats.tree_nodes_created > 0);
    assert(stats.simulations_performed > 0);
    assert(stats.max_tree_depth >= 0);
    assert(stats.average_playout_length > 0);
    
    std::cout << "  ✓ Tree growth test passed\n";
    std::cout << "    Max depth: " << stats.max_tree_depth << "\n";
    std::cout << "    Avg playout length: " << stats.average_playout_length << "\n";
}

void test_mcts_heuristic_playout() {
    std::cout << "Test: Heuristic playout\n";
    MCTSEngine engine;
    Board board;
    
    // Test that heuristic playout terminates
    // This is tested indirectly through simulation
    SearchLimits limits(0, 1000);
    Move move = engine.find_best_move(board, limits);
    
    const auto& stats = engine.get_mcts_stats();
    assert(stats.simulations_performed > 0);
    assert(stats.average_playout_length > 0);
    
    std::cout << "  ✓ Heuristic playout test passed\n";
}

void test_mcts_ucb1_selection() {
    std::cout << "Test: UCB1 selection\n";
    MCTSEngine engine;
    Board board;
    
    // Test that MCTS can select moves (UCB1 working)
    SearchLimits limits(0, 1500);
    Move move = engine.find_best_move(board, limits);
    
    const auto& stats = engine.get_mcts_stats();
    assert(stats.simulations_performed > 0);
    
    // Check that move statistics are collected
    if (!stats.move_visit_counts.empty()) {
        assert(stats.move_visit_counts.size() == stats.move_win_rates.size());
    }
    
    std::cout << "  ✓ UCB1 selection test passed\n";
}

void test_mcts_terminal_position() {
    std::cout << "Test: Terminal position handling\n";
    MCTSEngine engine;
    
    // Create a near-terminal position (most pieces placed)
    Board board;
    // Fill board to near-terminal state
    for (int i = 0; i < 50; ++i) {
        std::vector<int> moves;
        board.get_legal_moves(moves);
        if (moves.empty()) {
            board.pass();
            continue;
        }
        board.make_move(moves[0]);
    }
    
    SearchLimits limits(0, 500);
    Move move = engine.find_best_move(board, limits);
    
    // Should return a valid move or pass
    assert(move.is_valid() || move.is_pass());
    
    std::cout << "  ✓ Terminal position test passed\n";
}

void test_mcts_configuration() {
    std::cout << "Test: Configuration\n";
    
    // Test default configuration
    MCTSEngine engine1;
    assert(engine1.get_config().max_simulations == 10000);
    assert(engine1.get_config().ucb1_c > 0);
    
    // Test custom configuration
    MCTSEngine::Config config;
    config.max_simulations = 5000;
    config.max_time_ms = 3000;
    config.ucb1_c = 2.0;
    
    MCTSEngine engine2(config);
    assert(engine2.get_config().max_simulations == 5000);
    assert(engine2.get_config().max_time_ms == 3000);
    assert(engine2.get_config().ucb1_c == 2.0);
    
    std::cout << "  ✓ Configuration test passed\n";
}

void test_mcts_statistics() {
    std::cout << "Test: Statistics collection\n";
    MCTSEngine engine;
    Board board;
    SearchLimits limits(0, 2000);
    
    engine.find_best_move(board, limits);
    const auto& stats = engine.get_mcts_stats();
    
    assert(stats.simulations_performed > 0);
    assert(stats.time_elapsed_ms > 0);
    assert(stats.nodes_per_second >= 0);
    
    std::cout << "  ✓ Statistics test passed\n";
    std::cout << "    Time: " << stats.time_elapsed_ms << " ms\n";
    std::cout << "    Speed: " << stats.nodes_per_second << " sims/sec\n";
}

void test_mcts_reset() {
    std::cout << "Test: Reset functionality\n";
    MCTSEngine engine;
    Board board;
    SearchLimits limits(0, 1000);
    
    engine.find_best_move(board, limits);
    const auto& stats1 = engine.get_mcts_stats();
    assert(stats1.simulations_performed > 0);
    
    engine.reset();
    const auto& stats2 = engine.get_mcts_stats();
    assert(stats2.simulations_performed == 0);
    assert(stats2.tree_nodes_created == 0);
    
    std::cout << "  ✓ Reset test passed\n";
}

int main() {
    std::cout << "=== MCTS Engine Unit Tests ===\n\n";
    
    try {
        test_mcts_basic_search();
        test_mcts_tree_growth();
        test_mcts_heuristic_playout();
        test_mcts_ucb1_selection();
        test_mcts_terminal_position();
        test_mcts_configuration();
        test_mcts_statistics();
        test_mcts_reset();
        
        std::cout << "\n=== All MCTS tests passed! ===\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}

