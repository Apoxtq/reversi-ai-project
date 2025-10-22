/*
 * Week 3 Performance Benchmark
 * Tests Minimax engine performance at various depths
 * 
 * Author: Tianqixing
 * COMP390 Honours Year Project
 */

#include "core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include "ai/Evaluator.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <cmath>

using namespace reversi::core;
using namespace reversi::ai;

struct BenchmarkResult {
    int depth;
    int nodes;
    double time_ms;
    double nodes_per_sec;
    int best_move;
    int score;
};

BenchmarkResult bench_depth(const Board& board, int depth, bool use_ab) {
    MinimaxEngine::Config config{depth, use_ab};
    MinimaxEngine engine(config);
    
    auto start = std::chrono::high_resolution_clock::now();
    auto result = engine.find_best_move(board);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double time_ms = duration.count() / 1000.0;
    
    return BenchmarkResult{
        depth,
        result.nodes_searched,
        time_ms,
        result.nodes_per_sec(),
        result.best_move,
        result.score
    };
}

void print_result(const BenchmarkResult& r) {
    std::cout << std::setw(8) << r.depth;
    std::cout << std::setw(12) << r.nodes;
    std::cout << std::setw(12) << std::fixed << std::setprecision(2) << r.time_ms;
    std::cout << std::setw(14) << std::fixed << std::setprecision(2) << (r.nodes_per_sec / 1e6);
    std::cout << std::setw(12) << r.best_move;
    std::cout << std::setw(10) << r.score;
    std::cout << "\n";
}

void benchmark_alpha_beta_effect() {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Benchmark: Alpha-Beta Pruning Effectiveness                ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    Board board;  // Starting position
    
    std::cout << "\n  Depth   Nodes (No AB)   Nodes (With AB)   Reduction %\n";
    std::cout << "-------------------------------------------------------------\n";
    
    for (int depth = 3; depth <= 8; ++depth) {
        auto result_no_ab = bench_depth(board, depth, false);
        auto result_with_ab = bench_depth(board, depth, true);
        
        double reduction = 100.0 * (1.0 - static_cast<double>(result_with_ab.nodes) / result_no_ab.nodes);
        
        std::cout << std::setw(6) << depth;
        std::cout << std::setw(16) << result_no_ab.nodes;
        std::cout << std::setw(18) << result_with_ab.nodes;
        std::cout << std::setw(14) << std::fixed << std::setprecision(1) << reduction << "%";
        std::cout << "\n";
    }
}

void benchmark_depth_scaling() {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Benchmark: Search Depth Scaling (with Alpha-Beta)          ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    Board board;
    
    std::cout << "\n   Depth       Nodes   Time (ms)   Speed (M/s)   Best Move     Score\n";
    std::cout << "--------------------------------------------------------------------------\n";
    
    std::vector<BenchmarkResult> results;
    
    for (int depth = 1; depth <= 10; ++depth) {
        auto result = bench_depth(board, depth, true);
        results.push_back(result);
        print_result(result);
        
        // Stop if taking too long
        if (result.time_ms > 30000) {  // 30 seconds
            std::cout << "\n(Stopping benchmark - time limit exceeded)\n";
            break;
        }
    }
    
    // Calculate branching factor
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Effective Branching Factor Analysis:\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    for (size_t i = 1; i < results.size(); ++i) {
        double ebf = std::pow(
            static_cast<double>(results[i].nodes) / results[i-1].nodes,
            1.0
        );
        std::cout << "Depth " << results[i-1].depth << " → " << results[i].depth;
        std::cout << ": EBF ≈ " << std::fixed << std::setprecision(2) << ebf << "\n";
    }
}

void benchmark_different_positions() {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Benchmark: Performance on Different Board States           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    const int TEST_DEPTH = 6;
    
    // Test position 1: Initial position
    std::cout << "\n1. Initial Position (Opening)\n";
    std::cout << "   Depth       Nodes   Time (ms)   Speed (M/s)   Best Move     Score\n";
    std::cout << "--------------------------------------------------------------------------\n";
    Board board1;
    auto result1 = bench_depth(board1, TEST_DEPTH, true);
    print_result(result1);
    
    // Test position 2: After a few moves
    std::cout << "\n2. Mid-Game Position (after 8 moves)\n";
    std::cout << "   Depth       Nodes   Time (ms)   Speed (M/s)   Best Move     Score\n";
    std::cout << "--------------------------------------------------------------------------\n";
    Board board2;
    // Make some random moves
    for (int i = 0; i < 8; ++i) {
        auto moves = board2.get_legal_moves();
        if (!moves.empty()) {
            board2.make_move(moves[0]);
        }
    }
    auto result2 = bench_depth(board2, TEST_DEPTH, true);
    print_result(result2);
    
    // Test position 3: Late game
    std::cout << "\n3. End-Game Position (after 40 moves)\n";
    std::cout << "   Depth       Nodes   Time (ms)   Speed (M/s)   Best Move     Score\n";
    std::cout << "--------------------------------------------------------------------------\n";
    Board board3;
    for (int i = 0; i < 40; ++i) {
        auto moves = board3.get_legal_moves();
        if (moves.empty()) {
            board3.pass();
            moves = board3.get_legal_moves();
            if (moves.empty()) break;
        }
        board3.make_move(moves[0]);
    }
    auto result3 = bench_depth(board3, TEST_DEPTH, true);
    print_result(result3);
}

void benchmark_evaluator_speed() {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Benchmark: Evaluator Function Speed                        ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    Board board;
    const int ITERATIONS = 10000000;  // 10M evaluations
    
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile int dummy = 0;  // Prevent optimization
    for (int i = 0; i < ITERATIONS; ++i) {
        dummy += Evaluator::evaluate(board);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double time_sec = duration.count() / 1e6;
    double evals_per_sec = ITERATIONS / time_sec;
    
    std::cout << "\nEvaluations: " << ITERATIONS << "\n";
    std::cout << "Time: " << std::fixed << std::setprecision(3) << time_sec << " seconds\n";
    std::cout << "Speed: " << std::fixed << std::setprecision(2) << (evals_per_sec / 1e6) << " M evals/sec\n";
    std::cout << "Per-eval time: " << std::fixed << std::setprecision(2) << (time_sec * 1e9 / ITERATIONS) << " ns\n";
}

void performance_summary() {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Week 3 Performance Summary                                  ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    
    Board board;
    
    // Quick depth 6 benchmark
    auto result = bench_depth(board, 6, true);
    
    std::cout << "\nMinimax Engine @ Depth 6:\n";
    std::cout << "  ✓ Nodes searched: " << result.nodes << "\n";
    std::cout << "  ✓ Time: " << std::fixed << std::setprecision(2) << result.time_ms << " ms\n";
    std::cout << "  ✓ Speed: " << std::fixed << std::setprecision(2) << (result.nodes_per_sec / 1e6) << " M nodes/sec\n";
    
    bool meets_target = (result.nodes_per_sec / 1e6) >= 2.0;
    
    std::cout << "\nPerformance Target (≥2M nodes/sec): ";
    if (meets_target) {
        std::cout << "✅ PASSED\n";
    } else {
        std::cout << "❌ FAILED\n";
    }
    
    // Alpha-Beta effectiveness
    auto no_ab = bench_depth(board, 5, false);
    auto with_ab = bench_depth(board, 5, true);
    double reduction = 100.0 * (1.0 - static_cast<double>(with_ab.nodes) / no_ab.nodes);
    
    std::cout << "\nAlpha-Beta Pruning @ Depth 5:\n";
    std::cout << "  ✓ Reduction: " << std::fixed << std::setprecision(1) << reduction << "%\n";
    std::cout << "  ✓ Speedup: " << std::fixed << std::setprecision(2) << (static_cast<double>(no_ab.nodes) / with_ab.nodes) << "x\n";
    
    bool ab_effective = reduction >= 50.0;
    std::cout << "\nAlpha-Beta Target (≥50% reduction): ";
    if (ab_effective) {
        std::cout << "✅ PASSED\n";
    } else {
        std::cout << "❌ FAILED\n";
    }
}

int main(int argc, char* argv[]) {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Week 3 - Minimax Engine Performance Benchmark               ║\n";
    std::cout << "║  COMP390 Honours Year Project                                 ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n";
    
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--quick" || arg == "-q") {
            performance_summary();
            return 0;
        }
    }
    
    std::cout << "\nSelect benchmark:\n";
    std::cout << "  1. Quick Performance Summary\n";
    std::cout << "  2. Alpha-Beta Pruning Effectiveness\n";
    std::cout << "  3. Depth Scaling Analysis\n";
    std::cout << "  4. Different Board Positions\n";
    std::cout << "  5. Evaluator Speed Test\n";
    std::cout << "  6. Run All Benchmarks\n";
    std::cout << "\nChoice: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            performance_summary();
            break;
        case 2:
            benchmark_alpha_beta_effect();
            break;
        case 3:
            benchmark_depth_scaling();
            break;
        case 4:
            benchmark_different_positions();
            break;
        case 5:
            benchmark_evaluator_speed();
            break;
        case 6:
            performance_summary();
            benchmark_alpha_beta_effect();
            benchmark_depth_scaling();
            benchmark_different_positions();
            benchmark_evaluator_speed();
            break;
        default:
            std::cout << "Invalid choice!\n";
            return 1;
    }
    
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Benchmark complete!\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    
    return 0;
}

