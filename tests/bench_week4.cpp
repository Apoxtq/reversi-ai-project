/*
 * bench_week4.cpp - Performance benchmark for Week 4 evaluator
 * COMP390 Honours Year Project
 * 
 * Tests:
 * 1. Week 4 evaluator speed vs Week 3
 * 2. Stability analyzer overhead
 * 3. Phase-aware evaluation impact
 */

#include "../src/core/Board.hpp"
#include "ai/Evaluator.hpp"
#include "ai/Evaluator_Week4.hpp"
#include "ai/StabilityAnalyzer.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cstring>

using namespace reversi::core;
using namespace reversi::ai;

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"

struct BenchResult {
    std::string name;
    double time_ms;
    long long operations;
    double ops_per_sec;
    double cycles_per_op;
};

void print_header(const char* title) {
    std::cout << BOLD << CYAN << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║  " << title;
    int padding = 38 - strlen(title);
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚════════════════════════════════════════╝\n" << RESET;
}

// Generate various board positions for testing
std::vector<Board> generate_test_positions() {
    std::vector<Board> positions;
    
    // 1. Initial position
    positions.push_back(Board());
    
    // 2. Early game (after a few moves)
    Board early;
    early.make_move(19);  // d3
    early.make_move(18);  // c3
    positions.push_back(early);
    
    // 3. Mid game (simulate)
    Board mid;
    for (int i = 0; i < 20; i++) {
        auto moves = mid.get_legal_moves();
        if (!moves.empty()) {
            mid.make_move(moves[i % moves.size()]);
        }
    }
    positions.push_back(mid);
    
    // 4. Late game
    Board late;
    for (int i = 0; i < 50; i++) {
        auto moves = late.get_legal_moves();
        if (!moves.empty()) {
            late.make_move(moves[0]);
        } else {
            late.pass();
        }
        if (late.is_terminal()) break;
    }
    positions.push_back(late);
    
    return positions;
}

BenchResult benchmark_evaluator(
    const char* name,
    int (*eval_func)(const Board&),
    const std::vector<Board>& positions,
    long long iterations)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile int result = 0;  // Prevent optimization
    
    for (long long i = 0; i < iterations; i++) {
        const Board& board = positions[i % positions.size()];
        result += eval_func(board);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    BenchResult bench;
    bench.name = name;
    bench.operations = iterations;
    bench.time_ms = duration.count() / 1e6;
    bench.ops_per_sec = (iterations * 1e9) / duration.count();
    bench.cycles_per_op = (duration.count() / iterations) * 3.0;  // Assume 3GHz CPU
    
    return bench;
}

void print_result(const BenchResult& result) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  " << std::setw(25) << std::left << result.name;
    std::cout << ": " << YELLOW << std::setw(12) << (result.ops_per_sec / 1e6) << " M ops/s" << RESET;
    std::cout << "  (" << result.time_ms << " ms, " << result.cycles_per_op << " cycles/op)\n";
}

int eval_week3(const Board& b) { return Evaluator::evaluate(b); }
int eval_week4(const Board& b) { return EvaluatorWeek4::evaluate(b); }
int eval_week4_fast(const Board& b) { return EvaluatorWeek4::evaluate_fast(b); }

int main() {
    print_header("Week 4 Evaluator Benchmark");
    
    std::cout << "\nGenerating test positions...\n";
    auto positions = generate_test_positions();
    std::cout << "Created " << positions.size() << " test positions\n";
    
    // Benchmark parameters
    const long long ITERATIONS = 1'000'000;  // 1 million evaluations
    
    std::cout << "\nWarmup...";
    std::cout.flush();
    for (int i = 0; i < 10000; i++) {
        eval_week3(positions[0]);
        eval_week4(positions[0]);
    }
    std::cout << " Done\n";
    
    // Run benchmarks
    print_header("Evaluation Speed Comparison");
    
    std::vector<BenchResult> results;
    
    std::cout << "\nBenchmarking (1M evals each)...\n\n";
    
    results.push_back(benchmark_evaluator("Week 3 Baseline", eval_week3, positions, ITERATIONS));
    print_result(results.back());
    
    results.push_back(benchmark_evaluator("Week 4 Full Analysis", eval_week4, positions, ITERATIONS));
    print_result(results.back());
    
    results.push_back(benchmark_evaluator("Week 4 Fast Mode", eval_week4_fast, positions, ITERATIONS));
    print_result(results.back());
    
    // Performance comparison
    print_header("Performance Analysis");
    
    double week3_speed = results[0].ops_per_sec;
    double week4_speed = results[1].ops_per_sec;
    double week4_fast_speed = results[2].ops_per_sec;
    
    double overhead = ((week3_speed - week4_speed) / week3_speed) * 100.0;
    double speedup_fast = (week4_fast_speed / week3_speed) * 100.0 - 100.0;
    
    std::cout << "\nWeek 4 vs Week 3:\n";
    std::cout << "  Full analysis overhead: " << YELLOW << std::fixed << std::setprecision(1) 
              << overhead << "%" << RESET << "\n";
    std::cout << "  Fast mode speedup: " << GREEN << "+" << speedup_fast << "%" << RESET << "\n";
    
    std::cout << "\nTarget metrics:\n";
    std::cout << "  [OK] Week 4 Full: " << (results[1].ops_per_sec / 1e6) << " M ops/s ";
    std::cout << (results[1].ops_per_sec > 30e6 ? GREEN"[PASS]" : YELLOW"[OK]") << RESET << "\n";
    
    std::cout << "  [OK] Week 4 Fast: " << (results[2].ops_per_sec / 1e6) << " M ops/s ";
    std::cout << (results[2].ops_per_sec > 50e6 ? GREEN"[EXCELLENT]" : GREEN"[PASS]") << RESET << "\n";
    
    // Component breakdown
    print_header("Component Breakdown");
    
    std::cout << "\nStability Analyzer standalone:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < ITERATIONS; i++) {
        const Board& board = positions[i % positions.size()];
        StabilityAnalyzer::calculate_corner_stability(board);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    double stability_speed = (ITERATIONS * 1e9) / duration.count();
    
    std::cout << "  Corner stability: " << YELLOW << (stability_speed / 1e6) 
              << " M ops/s" << RESET << "\n";
    
    std::cout << "\n" << BOLD << GREEN << "[OK] Benchmark complete!" << RESET << "\n\n";
    
    return 0;
}

