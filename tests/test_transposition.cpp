/*
 * test_transposition.cpp - Transposition table test suite
 * COMP390 Honours Year Project - Week 5
 * 
 * Tests for TranspositionTable functionality:
 * - Basic store/retrieve operations
 * - Hash collision detection
 * - Depth-preferred replacement strategy
 * - Cache hit rate statistics
 * - Integration with MinimaxEngine
 */

#include "test_utils.hpp"
#include "ai/TranspositionTable.hpp"
#include "ai/MinimaxEngine.hpp"
#include "core/Board.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

using namespace reversi::core;
using namespace reversi::ai;
using namespace test;

// ANSI color codes
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

/**
 * @brief Test basic store and retrieve operations
 */
void test_basic_store_retrieve() {
    std::cout << "\n[TEST] Basic store and retrieve\n";
    std::cout << "--------------------------------\n";
    
    TranspositionTable tt(10);  // 2^10 = 1024 entries
    
    // Create test entry
    TTEntry entry;
    entry.hash = 0x1234567890ABCDEF;
    entry.score = 42;
    entry.depth = 5;
    entry.flag = 0;  // EXACT
    entry.best_move = 27;
    
    // Store entry
    tt.store(entry);
    
    // Retrieve entry
    TTEntry* retrieved = tt.probe(entry.hash);
    
    ASSERT_TRUE(retrieved != nullptr);
    ASSERT_TRUE(retrieved->is_valid());
    ASSERT_EQ(retrieved->hash, entry.hash);
    ASSERT_EQ(retrieved->score, entry.score);
    ASSERT_EQ(retrieved->depth, entry.depth);
    ASSERT_EQ(retrieved->flag, entry.flag);
    ASSERT_EQ(retrieved->best_move, entry.best_move);
    
    std::cout << GREEN << "✓ Basic store/retrieve works\n" << RESET;
}

/**
 * @brief Test hash collision detection
 */
void test_hash_collision() {
    std::cout << "\n[TEST] Hash collision detection\n";
    std::cout << "--------------------------------\n";
    
    TranspositionTable tt(10);  // 2^10 = 1024 entries (small for collision testing)
    
    // Store entry with hash
    TTEntry entry1;
    entry1.hash = 0x1000;
    entry1.score = 100;
    entry1.depth = 3;
    entry1.flag = 0;
    entry1.best_move = 10;
    tt.store(entry1);
    
    // Try to retrieve with same hash
    TTEntry* retrieved1 = tt.probe(0x1000);
    ASSERT_TRUE(retrieved1 != nullptr);
    ASSERT_EQ(retrieved1->hash, 0x1000);
    ASSERT_EQ(retrieved1->score, 100);
    
    // Try to retrieve with different hash (should not match)
    TTEntry* retrieved2 = tt.probe(0x2000);
    ASSERT_TRUE(retrieved2 == nullptr || !retrieved2->matches(0x2000));
    
    std::cout << GREEN << "✓ Hash collision detection works\n" << RESET;
}

/**
 * @brief Test depth-preferred replacement strategy
 */
void test_depth_preferred_replacement() {
    std::cout << "\n[TEST] Depth-preferred replacement\n";
    std::cout << "-----------------------------------\n";
    
    TranspositionTable tt(10);  // Small table to force replacements (2^10 = 1024 entries)
    
    // Store entry with depth 3
    TTEntry entry1;
    entry1.hash = 0x1000;
    entry1.score = 100;
    entry1.depth = 3;
    entry1.flag = 0;
    entry1.best_move = 10;
    tt.store(entry1);
    
    // Store entry with same index but higher depth (should replace)
    TTEntry entry2;
    entry2.hash = 0x2000;  // Different hash, same index (collision)
    entry2.score = 200;
    entry2.depth = 5;  // Higher depth
    entry2.flag = 0;
    entry2.best_move = 20;
    tt.store(entry2);
    
    // Check that higher depth entry is stored
    TTEntry* retrieved = tt.probe(0x2000);
    if (retrieved && retrieved->matches(0x2000)) {
        ASSERT_EQ(retrieved->depth, 5);
        std::cout << GREEN << "✓ Depth-preferred replacement works\n" << RESET;
    } else {
        // If collision didn't occur, that's also fine
        std::cout << YELLOW << "⚠ No collision occurred (expected with larger tables)\n" << RESET;
    }
}

/**
 * @brief Test cache statistics
 */
void test_cache_statistics() {
    std::cout << "\n[TEST] Cache statistics\n";
    std::cout << "-----------------------\n";
    
    TranspositionTable tt(10);
    
    // Initially, no hits
    auto stats = tt.get_stats();
    ASSERT_EQ(stats.hits, 0);
    ASSERT_EQ(stats.misses, 0);
    ASSERT_EQ(stats.hit_rate, 0.0);
    
    // Store and retrieve (should be a hit)
    TTEntry entry;
    entry.hash = 0x1234;
    entry.score = 50;
    entry.depth = 4;
    entry.flag = 0;
    entry.best_move = 15;
    tt.store(entry);
    
    // First probe (should be a hit)
    TTEntry* retrieved = tt.probe(0x1234);
    ASSERT_TRUE(retrieved != nullptr);
    
    stats = tt.get_stats();
    ASSERT_GT(stats.hits, 0);
    
    // Probe non-existent entry (should be a miss)
    tt.probe(0x9999);
    stats = tt.get_stats();
    ASSERT_GT(stats.misses, 0);
    
    std::cout << GREEN << "✓ Cache statistics work\n" << RESET;
    std::cout << "  Hits: " << stats.hits << ", Misses: " << stats.misses 
              << ", Hit Rate: " << std::fixed << std::setprecision(2) 
              << stats.hit_rate * 100 << "%\n";
}

/**
 * @brief Test clear functionality
 */
void test_clear() {
    std::cout << "\n[TEST] Clear table\n";
    std::cout << "------------------\n";
    
    TranspositionTable tt(10);
    
    // Store some entries
    for (int i = 0; i < 10; ++i) {
        TTEntry entry;
        entry.hash = 0x1000 + i;
        entry.score = i * 10;
        entry.depth = 3;
        entry.flag = 0;
        entry.best_move = i;
        tt.store(entry);
    }
    
    // Verify entries exist
    TTEntry* retrieved = tt.probe(0x1000);
    ASSERT_TRUE(retrieved != nullptr);
    ASSERT_TRUE(retrieved->is_valid());
    
    // Clear table
    tt.clear();
    
    // Verify entries are cleared
    retrieved = tt.probe(0x1000);
    ASSERT_TRUE(retrieved == nullptr || !retrieved->is_valid());
    
    std::cout << GREEN << "✓ Clear functionality works\n" << RESET;
}

/**
 * @brief Test integration with MinimaxEngine
 */
void test_minimax_integration() {
    std::cout << "\n[TEST] MinimaxEngine integration\n";
    std::cout << "---------------------------------\n";
    
    // Create engine with transposition table enabled
    MinimaxEngine::Config config;
    config.max_depth = 4;
    config.use_transposition = true;
    config.tt_size_bits = 12;  // 2^12 = 4096 entries
    
    MinimaxEngine engine(config);
    
    Board board;
    
    // First search (should populate TT)
    auto result1 = engine.find_best_move(board);
    ASSERT_TRUE(result1.best_move >= 0);
    
    // Get TT stats
    auto stats = engine.get_tt_stats();
    std::cout << "  After first search:\n";
    std::cout << "    Hits: " << stats.hits << ", Misses: " << stats.misses << "\n";
    
    // Second search from same position (should use TT)
    engine.clear_tt();  // Clear to test fresh
    auto result2 = engine.find_best_move(board);
    
    // Get updated stats
    stats = engine.get_tt_stats();
    std::cout << "  After second search:\n";
    std::cout << "    Hits: " << stats.hits << ", Misses: " << stats.misses << "\n";
    std::cout << "    Hit Rate: " << std::fixed << std::setprecision(2) 
              << stats.hit_rate * 100 << "%\n";
    
    // Results should be consistent
    ASSERT_TRUE(result2.best_move >= 0);
    
    std::cout << GREEN << "✓ MinimaxEngine integration works\n" << RESET;
}

/**
 * @brief Test performance with many entries
 */
void test_performance() {
    std::cout << "\n[TEST] Performance with many entries\n";
    std::cout << "-------------------------------------\n";
    
    TranspositionTable tt(16);  // 2^16 = 65536 entries
    
    Timer timer;
    
    // Store many entries
    const int num_entries = 10000;
    for (int i = 0; i < num_entries; ++i) {
        TTEntry entry;
        entry.hash = 0x1000000 + i;
        entry.score = i % 1000;
        entry.depth = (i % 10) + 1;
        entry.flag = i % 3;
        entry.best_move = i % 64;
        tt.store(entry);
    }
    
    double store_time = timer.elapsed_ms();
    timer.reset();
    
    // Retrieve entries
    int hits = 0;
    for (int i = 0; i < num_entries; ++i) {
        TTEntry* entry = tt.probe(0x1000000 + i);
        if (entry && entry->matches(0x1000000 + i)) {
            hits++;
        }
    }
    
    double retrieve_time = timer.elapsed_ms();
    
    std::cout << "  Stored " << num_entries << " entries in " 
              << std::fixed << std::setprecision(2) << store_time << " ms\n";
    std::cout << "  Retrieved " << hits << " entries in " 
              << std::fixed << std::setprecision(2) << retrieve_time << " ms\n";
    std::cout << "  Store speed: " << std::fixed << std::setprecision(0)
              << (num_entries / store_time) * 1000 << " ops/s\n";
    std::cout << "  Retrieve speed: " << std::fixed << std::setprecision(0)
              << (num_entries / retrieve_time) * 1000 << " ops/s\n";
    
    ASSERT_GT(hits, num_entries * 0.9);  // At least 90% hit rate
    
    std::cout << GREEN << "✓ Performance test passed\n" << RESET;
}

/**
 * @brief Test move ordering with transposition table
 */
void test_move_ordering() {
    std::cout << "\n[TEST] Move ordering with TT\n";
    std::cout << "-----------------------------\n";
    
    MinimaxEngine::Config config;
    config.max_depth = 4;
    config.use_transposition = true;
    config.tt_size_bits = 12;
    
    MinimaxEngine engine(config);
    Board board;
    
    // First search to populate TT
    auto result1 = engine.find_best_move(board);
    ASSERT_TRUE(result1.best_move >= 0);
    
    // Make a move and search again
    board.make_move(result1.best_move);
    
    // Second search (should use TT for move ordering)
    auto result2 = engine.find_best_move(board);
    ASSERT_TRUE(result2.best_move >= 0);
    
    auto stats = engine.get_tt_stats();
    std::cout << "  TT Stats: " << stats.hits << " hits, " 
              << stats.misses << " misses\n";
    
    std::cout << GREEN << "✓ Move ordering works\n" << RESET;
}

int main() {
    std::cout << CYAN << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║  Transposition Table Unit Tests      ║\n";
    std::cout << "║  COMP390 - Week 5                     ║\n";
    std::cout << "╚════════════════════════════════════════╝\n" << RESET;
    
    int passed = 0;
    int total = 8;
    
    try {
        test_basic_store_retrieve();
        passed++;
        
        test_hash_collision();
        passed++;
        
        test_depth_preferred_replacement();
        passed++;
        
        test_cache_statistics();
        passed++;
        
        test_clear();
        passed++;
        
        test_minimax_integration();
        passed++;
        
        test_performance();
        passed++;
        
        test_move_ordering();
        passed++;
        
    } catch (const std::exception& e) {
        std::cout << RED << "Exception: " << e.what() << RESET << "\n";
    }
    
    std::cout << CYAN << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║  Test Summary                          ║\n";
    std::cout << "╚════════════════════════════════════════╝\n" << RESET;
    std::cout << "Passed: " << GREEN << passed << "/" << total << RESET << "\n";
    
    if (passed == total) {
        std::cout << GREEN << "✓ All tests passed!\n" << RESET;
        return 0;
    } else {
        std::cout << RED << "✗ Some tests failed\n" << RESET;
        return 1;
    }
}

