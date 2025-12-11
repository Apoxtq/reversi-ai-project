/*
 * test_benchmark_week10.cpp - Comprehensive Test Suite for Week 10 Benchmark Framework
 * COMP390 Honours Year Project - Week 10
 * 
 * Comprehensive tests for:
 * - MatchEngine
 * - Statistics
 * - PositionSuite
 * - Benchmark framework
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "../src/research/benchmark/MatchEngine.hpp"
#include "../src/research/benchmark/Statistics.hpp"
#include "../src/research/benchmark/PositionSuite.hpp"
#include "../src/research/benchmark/Benchmark.hpp"
#include "../src/ai/MinimaxEngine.hpp"
#include "../src/ai/MCTSEngine.hpp"
#include "../src/core/Board.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>

using namespace reversi::research;
using namespace reversi::ai;
using namespace reversi::core;

// Test utilities
int tests_passed = 0;
int tests_failed = 0;

#define TEST(name) \
    do { \
        std::cout << "Testing: " << name << "... "; \
        try {

#define ASSERT(condition) \
            if (!(condition)) { \
                std::cout << "FAILED (assertion: " << #condition << ")\n"; \
                tests_failed++; \
                return; \
            }

#define END_TEST \
            std::cout << "PASSED\n"; \
            tests_passed++; \
        } catch (const std::exception& e) { \
            std::cout << "FAILED (exception: " << e.what() << ")\n"; \
            tests_failed++; \
        } \
    } while (0)

// ==================== Statistics Tests ====================

void test_statistics_basic() {
    TEST("Statistics - Basic calculation") {
        std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
        auto stats = Statistics::calculate(data);
        
        ASSERT(stats.sample_size == 5);
        ASSERT(std::abs(stats.mean - 3.0) < 0.001);
        ASSERT(std::abs(stats.median - 3.0) < 0.001);
        ASSERT(std::abs(stats.min - 1.0) < 0.001);
        ASSERT(std::abs(stats.max - 5.0) < 0.001);
        ASSERT(stats.std_dev > 0.0);
    } END_TEST;
}

void test_statistics_confidence_interval() {
    TEST("Statistics - Confidence interval") {
        std::vector<double> data = {10.0, 12.0, 14.0, 16.0, 18.0};
        auto stats = Statistics::calculate(data);
        
        ASSERT(stats.ci_95_lower < stats.mean);
        ASSERT(stats.ci_95_upper > stats.mean);
        ASSERT(stats.ci_95_lower < stats.ci_95_upper);
    } END_TEST;
}

void test_statistics_formatting() {
    TEST("Statistics - Formatting") {
        std::vector<double> data = {1.0, 2.0, 3.0};
        auto stats = Statistics::calculate(data);
        
        std::string mean_std = Statistics::format_mean_std(stats);
        ASSERT(!mean_std.empty());
        ASSERT(mean_std.find("±") != std::string::npos);
        
        std::string mean_ci = Statistics::format_mean_ci(stats);
        ASSERT(!mean_ci.empty());
        ASSERT(mean_ci.find("[") != std::string::npos);
    } END_TEST;
}

void test_statistics_empty() {
    TEST("Statistics - Empty data") {
        std::vector<double> data;
        auto stats = Statistics::calculate(data);
        
        ASSERT(stats.sample_size == 0);
        ASSERT(!stats.is_valid());
    } END_TEST;
}

// ==================== PositionSuite Tests ====================

void test_position_suite_generation() {
    TEST("PositionSuite - Generate opening positions") {
        auto positions = PositionSuite::generate_opening(5, 4, 6, 12345);
        
        ASSERT(positions.size() == 5);
        for (const auto& pos : positions) {
            ASSERT(!pos.is_terminal());
        }
    } END_TEST;
}

void test_position_suite_midgame() {
    TEST("PositionSuite - Generate midgame positions") {
        auto positions = PositionSuite::generate_midgame(5, 10, 20, 12345);
        
        ASSERT(positions.size() == 5);
        for (const auto& pos : positions) {
            ASSERT(!pos.is_terminal());
        }
    } END_TEST;
}

void test_position_suite_standard_64() {
    TEST("PositionSuite - Generate standard 64 suite") {
        auto positions = PositionSuite::generate_standard_64(12345);
        
        ASSERT(positions.size() == 64);
        for (const auto& pos : positions) {
            ASSERT(!pos.is_terminal());
        }
    } END_TEST;
}

// ==================== MatchEngine Tests ====================

void test_match_engine_single_game() {
    TEST("MatchEngine - Single game") {
        auto minimax = std::make_shared<MinimaxEngine>();
        auto mcts = std::make_shared<MCTSEngine>();
        
        SearchLimits limits;
        limits.max_depth = 3;
        limits.max_time_ms = 1000;
        
        MatchEngine::MatchConfig config;
        config.verbose = false;
        
        auto result = MatchEngine::play_single_game(
            minimax, mcts, true, limits, config
        );
        
        ASSERT(result.move_count > 0);
        ASSERT(result.duration_ms > 0.0);
        ASSERT(result.winner == 1 || result.winner == -1 || result.winner == 0);
    } END_TEST;
}

void test_match_engine_match() {
    TEST("MatchEngine - Complete match") {
        auto minimax = std::make_shared<MinimaxEngine>();
        auto mcts = std::make_shared<MCTSEngine>();
        
        MatchEngine::MatchConfig config;
        config.num_games = 2; // Small number for testing
        config.alternate_colors = true;
        config.verbose = false;
        
        auto result = MatchEngine::play_match(minimax, mcts, config);
        
        ASSERT(result.total_games() == 2);
        ASSERT(result.player1_wins + result.player2_wins + result.draws == 2);
        ASSERT(result.avg_game_length > 0.0);
    } END_TEST;
}

void test_match_engine_color_alternation() {
    TEST("MatchEngine - Color alternation") {
        auto minimax = std::make_shared<MinimaxEngine>();
        auto mcts = std::make_shared<MCTSEngine>();
        
        MatchEngine::MatchConfig config;
        config.num_games = 4;
        config.alternate_colors = true;
        config.verbose = false;
        
        auto result = MatchEngine::play_match(minimax, mcts, config);
        
        ASSERT(result.games.size() == 4);
        // Check that colors alternate
        bool first_black = result.games[0].player1_was_black;
        bool second_black = result.games[1].player1_was_black;
        ASSERT(first_black != second_black);
    } END_TEST;
}

// ==================== Benchmark Tests ====================

void test_benchmark_single() {
    TEST("Benchmark - Single algorithm") {
        auto minimax = std::make_shared<MinimaxEngine>();
        
        BenchmarkConfig config;
        config.num_positions = 5;
        config.repetitions = 1;
        config.suite_type = PositionSuite::SuiteType::OPENING;
        config.search_limits.max_depth = 3;
        config.search_limits.max_time_ms = 1000;
        
        auto result = Benchmark::run_single(minimax, config);
        
        ASSERT(result.positions_tested == 5);
        ASSERT(result.total_nodes > 0);
        ASSERT(result.nodes_per_second > 0.0);
    } END_TEST;
}

void test_benchmark_comparison() {
    TEST("Benchmark - Algorithm comparison") {
        auto minimax = std::make_shared<MinimaxEngine>();
        auto mcts = std::make_shared<MCTSEngine>();
        
        std::vector<std::shared_ptr<AIStrategy>> strategies = {minimax, mcts};
        
        BenchmarkConfig config;
        config.num_positions = 3;
        config.repetitions = 1;
        config.suite_type = PositionSuite::SuiteType::OPENING;
        config.search_limits.max_depth = 3;
        config.search_limits.max_time_ms = 1000;
        
        auto results = Benchmark::run_comparison(strategies, config);
        
        ASSERT(results.size() == 2);
        ASSERT(results[0].algorithm_name == minimax->get_name());
        ASSERT(results[1].algorithm_name == mcts->get_name());
    } END_TEST;
}

void test_benchmark_csv_export() {
    TEST("Benchmark - CSV export") {
        auto minimax = std::make_shared<MinimaxEngine>();
        
        BenchmarkConfig config;
        config.num_positions = 3;
        config.repetitions = 1;
        config.suite_type = PositionSuite::SuiteType::OPENING;
        config.search_limits.max_depth = 3;
        config.search_limits.max_time_ms = 1000;
        
        auto result = Benchmark::run_single(minimax, config);
        
        std::string filename = "test_benchmark_export.csv";
        Benchmark::export_results_csv({result}, filename);
        
        // Check file exists (basic check)
        std::ifstream file(filename);
        ASSERT(file.good());
        file.close();
        
        // Clean up
        std::remove(filename.c_str());
    } END_TEST;
}

// ==================== Integration Tests ====================

void test_integration_minimax_vs_mcts() {
    TEST("Integration - Minimax vs MCTS comparison") {
        SearchLimits minimax_limits;
        minimax_limits.max_depth = 3;
        minimax_limits.max_time_ms = 1000;
        
        SearchLimits mcts_limits;
        mcts_limits.max_time_ms = 1000;
        
        auto result = Benchmark::compare_minimax_vs_mcts(
            minimax_limits, mcts_limits, 2, true
        );
        
        ASSERT(result.total_games() == 2);
        ASSERT(result.player1_name == "MinimaxEngine" || result.player1_name == "MCTSEngine");
    } END_TEST;
}

void test_integration_difficulty_levels() {
    TEST("Integration - Difficulty level testing") {
        auto minimax = std::make_shared<MinimaxEngine>();
        
        std::vector<Difficulty> levels = {Difficulty::EASY, Difficulty::MEDIUM};
        
        BenchmarkConfig config;
        config.num_positions = 3;
        config.suite_type = PositionSuite::SuiteType::OPENING;
        
        auto results = Benchmark::test_difficulty_levels(minimax, levels, config);
        
        ASSERT(results.size() == 2);
    } END_TEST;
}

// ==================== Main Test Runner ====================

int main() {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Week 10 Benchmark Framework Test Suite\n";
    std::cout << "COMP390 Honours Year Project\n";
    std::cout << std::string(60, '=') << "\n\n";
    
    // Statistics tests
    test_statistics_basic();
    test_statistics_confidence_interval();
    test_statistics_formatting();
    test_statistics_empty();
    
    // PositionSuite tests
    test_position_suite_generation();
    test_position_suite_midgame();
    test_position_suite_standard_64();
    
    // MatchEngine tests
    test_match_engine_single_game();
    test_match_engine_match();
    test_match_engine_color_alternation();
    
    // Benchmark tests
    test_benchmark_single();
    test_benchmark_comparison();
    test_benchmark_csv_export();
    
    // Integration tests
    test_integration_minimax_vs_mcts();
    test_integration_difficulty_levels();
    
    // Summary
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "Test Summary\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "Passed: " << tests_passed << "\n";
    std::cout << "Failed: " << tests_failed << "\n";
    std::cout << "Total:  " << (tests_passed + tests_failed) << "\n";
    
    if (tests_failed == 0) {
        std::cout << "\n✓ All tests passed!\n";
        return 0;
    } else {
        std::cout << "\n✗ Some tests failed!\n";
        return 1;
    }
}

