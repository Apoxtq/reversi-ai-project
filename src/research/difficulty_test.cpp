/*
 * difficulty_test.cpp - Difficulty Level Testing
 * COMP390 Honours Year Project - Week 10
 * 
 * Tests algorithm performance at different difficulty levels
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "benchmark/Benchmark.hpp"
#include "benchmark/Statistics.hpp"
#include "ai/MinimaxEngine.hpp"
#include "ai/MCTSEngine.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace reversi::research;
using namespace reversi::ai;

void print_benchmark_result(const BenchmarkResult& result) {
    std::cout << "\n" << std::string(60, '-') << "\n";
    std::cout << "Algorithm: " << result.algorithm_name << "\n";
    std::cout << std::string(60, '-') << "\n";
    
    std::cout << "Positions tested: " << result.positions_tested << "\n";
    std::cout << "Total nodes: " << result.total_nodes << "\n";
    std::cout << "Total time: " << result.total_time_ms << " ms\n";
    std::cout << "Nodes per second: " << std::fixed << std::setprecision(0) 
              << result.nodes_per_second << "\n";
    std::cout << "Average depth: " << result.avg_depth_reached << "\n";
    
    std::cout << "\nTime per position: " 
              << Statistics::format_mean_std(result.time_stats, 2) << " ms\n";
    std::cout << "Nodes per position: " 
              << Statistics::format_mean_std(result.nodes_stats, 0) << "\n";
    std::cout << "Depth reached: " 
              << Statistics::format_mean_std(result.depth_stats, 1) << "\n";
    
    std::cout << std::string(60, '-') << "\n";
}

int main(int argc, char* argv[]) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Difficulty Level Testing\n";
    std::cout << "COMP390 Honours Year Project - Week 10\n";
    std::cout << std::string(60, '=') << "\n\n";
    
    // Parse arguments
    std::string algorithm = "minimax";
    if (argc > 1) {
        algorithm = argv[1];
    }
    
    int num_positions = 20;
    if (argc > 2) {
        num_positions = std::atoi(argv[2]);
    }
    
    std::cout << "Configuration:\n";
    std::cout << "  Algorithm: " << algorithm << "\n";
    std::cout << "  Positions per level: " << num_positions << "\n";
    std::cout << "\nStarting tests...\n\n";
    
    // Create AI strategy
    std::shared_ptr<AIStrategy> strategy;
    if (algorithm == "mcts") {
        strategy = std::make_shared<MCTSEngine>();
    } else {
        strategy = std::make_shared<MinimaxEngine>();
    }
    
    // Test difficulty levels
    std::vector<Difficulty> levels = {
        Difficulty::EASY,
        Difficulty::MEDIUM,
        Difficulty::HARD
    };
    
    // Configure benchmark
    BenchmarkConfig config;
    config.num_positions = num_positions;
    config.suite_type = PositionSuite::SuiteType::STANDARD_64;
    config.repetitions = 1;
    
    // Run tests
    std::vector<BenchmarkResult> results = Benchmark::test_difficulty_levels(
        strategy,
        levels,
        config
    );
    
    // Print results
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Results Summary\n";
    std::cout << std::string(60, '=') << "\n";
    
    for (const auto& result : results) {
        print_benchmark_result(result);
    }
    
    // Export to CSV
    std::string csv_filename = "difficulty_test_" + algorithm + "_results.csv";
    Benchmark::export_results_csv(results, csv_filename);
    std::cout << "\nResults exported to: " << csv_filename << "\n";
    
    std::cout << "\nTesting completed successfully!\n";
    return 0;
}

