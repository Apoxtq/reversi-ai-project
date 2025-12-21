/*
 * Performance Benchmark Suite
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Measures performance of optimized bitboard operations
 */

#include "../src/core/Board.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <random>

using namespace reversi::core;
using namespace std::chrono;

// ==================== Benchmark Utilities ====================

class Timer {
    high_resolution_clock::time_point start_time;
public:
    Timer() : start_time(high_resolution_clock::now()) {}
    
    double elapsed_ms() const {
        auto end = high_resolution_clock::now();
        return duration<double, std::milli>(end - start_time).count();
    }
    
    double elapsed_sec() const {
        return elapsed_ms() / 1000.0;
    }
};

void print_header(const std::string& title) {
    std::cout << "\n";
    std::cout << "+" << std::string(60, '=') << "+\n";
    std::cout << "| " << std::left << std::setw(58) << title << " |\n";
    std::cout << "+" << std::string(60, '=') << "+\n\n";
}

void print_result(const std::string& test_name, double ops_per_sec, const std::string& unit = "ops/sec") {
    std::cout << std::left << std::setw(35) << test_name << ": ";
    std::cout << std::right << std::setw(12) << std::fixed << std::setprecision(0) << ops_per_sec;
    std::cout << " " << unit << "\n";
}

// ==================== Benchmark Tests ====================

void benchmark_legal_moves() {
    print_header("Benchmark: Legal Move Generation");
    
    Board board;
    const int ITERATIONS = 1000000;
    
    // Warm-up
    for (int i = 0; i < 1000; ++i) {
        volatile uint64_t moves = board.legal_moves();
        (void)moves;
    }
    
    // Benchmark
    Timer timer;
    uint64_t total_moves = 0;
    
    for (int i = 0; i < ITERATIONS; ++i) {
        uint64_t moves = board.legal_moves();
        total_moves += std::popcount(moves);
    }
    
    double elapsed = timer.elapsed_sec();
    double ops_per_sec = ITERATIONS / elapsed;
    
    print_result("Legal move generation", ops_per_sec);
    std::cout << "Total legal moves found: " << total_moves << "\n";
    std::cout << "Average time per call: " << (elapsed * 1e6 / ITERATIONS) << " us\n";
}

void benchmark_make_move() {
    print_header("Benchmark: Move Execution");
    
    const int ITERATIONS = 500000;
    const int MOVES_PER_GAME = 10;
    
    // Warm-up
    for (int i = 0; i < 100; ++i) {
        Board board;
        auto moves = board.get_legal_moves();
        if (!moves.empty()) {
            board.make_move(moves[0]);
        }
    }
    
    // Benchmark
    Timer timer;
    int total_moves_made = 0;
    
    for (int i = 0; i < ITERATIONS; ++i) {
        Board board;
        
        for (int j = 0; j < MOVES_PER_GAME && !board.is_terminal(); ++j) {
            auto moves = board.get_legal_moves();
            if (moves.empty()) {
                board.pass();
                moves = board.get_legal_moves();
                if (moves.empty()) break;
            }
            
            board.make_move(moves[i % moves.size()]);
            total_moves_made++;
        }
    }
    
    double elapsed = timer.elapsed_sec();
    double ops_per_sec = total_moves_made / elapsed;
    
    print_result("Make move operations", ops_per_sec);
    std::cout << "Total moves executed: " << total_moves_made << "\n";
    std::cout << "Average time per move: " << (elapsed * 1e6 / total_moves_made) << " us\n";
}

void benchmark_position_evaluation() {
    print_header("Benchmark: Position Evaluation (Full Game Simulation)");
    
    const int NUM_GAMES = 10000;
    
    Timer timer;
    int total_moves = 0;
    int total_nodes = 0;
    
    std::mt19937 rng(42);
    
    for (int game = 0; game < NUM_GAMES; ++game) {
        Board board;
        
        while (!board.is_terminal()) {
            auto moves = board.get_legal_moves();
            
            if (moves.empty()) {
                board.pass();
                moves = board.get_legal_moves();
                if (moves.empty()) break;
            }
            
            // Random move selection
            int move_idx = rng() % moves.size();
            board.make_move(moves[move_idx]);
            total_moves++;
            total_nodes++;
        }
    }
    
    double elapsed = timer.elapsed_sec();
    double games_per_sec = NUM_GAMES / elapsed;
    double moves_per_sec = total_moves / elapsed;
    
    print_result("Games simulated", games_per_sec, "games/sec");
    print_result("Moves per second", moves_per_sec, "moves/sec");
    std::cout << "Average moves per game: " << (total_moves / (double)NUM_GAMES) << "\n";
    std::cout << "Total time: " << std::fixed << std::setprecision(2) << elapsed << " sec\n";
}

void benchmark_zobrist_hash() {
    print_header("Benchmark: Zobrist Hash Computation");
    
    Board board;
    const int ITERATIONS = 5000000;
    
    // Warm-up
    for (int i = 0; i < 1000; ++i) {
        volatile uint64_t h = board.hash();
        (void)h;
    }
    
    // Benchmark
    Timer timer;
    uint64_t hash_sum = 0;
    
    for (int i = 0; i < ITERATIONS; ++i) {
        hash_sum ^= board.hash();
    }
    
    double elapsed = timer.elapsed_sec();
    double ops_per_sec = ITERATIONS / elapsed;
    
    print_result("Hash computations", ops_per_sec);
    std::cout << "Checksum: 0x" << std::hex << hash_sum << std::dec << "\n";
    std::cout << "Average time per hash: " << (elapsed * 1e9 / ITERATIONS) << " ns\n";
}

void benchmark_memory_operations() {
    print_header("Benchmark: Memory & Copy Operations");
    
    const int ITERATIONS = 10000000;
    Board original;
    
    // Test copy constructor
    Timer timer1;
    for (int i = 0; i < ITERATIONS; ++i) {
        Board copy = original.copy();
        // Prevent optimization
        if (copy.player == 0xFFFFFFFFFFFFFFFFULL) {
            std::cout << "Unlikely\n";
        }
    }
    double copy_ops = ITERATIONS / timer1.elapsed_sec();
    
    // Test copy method
    Board dest;
    Timer timer2;
    for (int i = 0; i < ITERATIONS; ++i) {
        original.copy(&dest);
        if (dest.player == 0xFFFFFFFFFFFFFFFFULL) {
            std::cout << "Unlikely\n";
        }
    }
    double copy_method_ops = ITERATIONS / timer2.elapsed_sec();
    
    print_result("Board copy (constructor)", copy_ops);
    print_result("Board copy (method)", copy_method_ops);
}

void test_hash_quality() {
    print_header("Test: Zobrist Hash Quality (Collision Detection)");
    
    const int NUM_POSITIONS = 100000;
    std::vector<uint64_t> hashes;
    hashes.reserve(NUM_POSITIONS);
    
    std::mt19937 rng(12345);
    
    // Generate random positions
    Timer timer;
    int skipped = 0;
    for (int i = 0; i < NUM_POSITIONS; ++i) {
        Board board;
        
        // Play random moves
        int moves_to_play = 5 + (rng() % 30);  // 5-35 moves
        for (int j = 0; j < moves_to_play && !board.is_terminal(); ++j) {
            auto moves = board.get_legal_moves();
            if (moves.empty()) {
                board.pass();
                moves = board.get_legal_moves();
                if (moves.empty()) break;
            }
            board.make_move(moves[rng() % moves.size()]);
        }
        
        // Skip identical starting positions
        if (board.player == 0x0000000810000000ULL && board.opponent == 0x0000001008000000ULL) {
            skipped++;
            continue;
        }
        
        hashes.push_back(board.hash());
    }
    
    // Sort and check for collisions
    std::sort(hashes.begin(), hashes.end());
    int collisions = 0;
    for (size_t i = 1; i < hashes.size(); ++i) {
        if (hashes[i] == hashes[i-1]) {
            collisions++;
        }
    }
    
    double elapsed = timer.elapsed_sec();
    
    std::cout << "Positions tested: " << hashes.size() << " (skipped " << skipped << " identical)\n";
    std::cout << "Hash collisions: " << collisions << " (" 
              << (100.0 * collisions / hashes.size()) << "%)\n";
    std::cout << "Time taken: " << std::fixed << std::setprecision(2) << elapsed << " sec\n";
    
    if (collisions == 0) {
        std::cout << "[OK] EXCELLENT: No collisions detected!\n";
    } else if (collisions < hashes.size() / 1000) {
        std::cout << "[OK] GOOD: Very low collision rate (< 0.1%)\n";
    } else if (collisions < hashes.size() / 100) {
        std::cout << "[OK] ACCEPTABLE: Low collision rate (< 1%)\n";
    } else {
        std::cout << "[WARN] WARNING: High collision rate detected\n";
    }
}

void compare_with_baseline() {
    print_header("Performance Summary & Targets");
    
    std::cout << "Expected Performance Targets:\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << "Legal move generation:   > 10,000,000 ops/sec\n";
    std::cout << "Move execution:          >  5,000,000 ops/sec\n";
    std::cout << "Hash computation:        > 20,000,000 ops/sec\n";
    std::cout << "Full game simulation:    >  5,000    games/sec\n";
    std::cout << "-----------------------------------------------\n\n";
    
    std::cout << "Optimization Techniques Applied:\n";
    std::cout << "  * Kogge-Stone parallel prefix algorithm\n";
    std::cout << "  * Zobrist hashing for O(1) position comparison\n";
    std::cout << "  * Branchless bitwise operations\n";
    std::cout << "  * Edge mask pre-computation\n";
    std::cout << "  * Memory-efficient move generation\n";
    std::cout << "  * Cache-friendly data structures (16 bytes per board)\n";
}

// ==================== Main Benchmark Runner ====================

int main() {
    std::cout << "\n";
    std::cout << "+============================================================+\n";
    std::cout << "|     Reversi Bitboard Performance Benchmark Suite           |\n";
    std::cout << "|     COMP390 Honours Year Project (2025-26)                 |\n";
    std::cout << "|     Student: Tianqixing (201821852)                        |\n";
    std::cout << "+============================================================+\n";
    
    try {
        // Run all benchmarks
        benchmark_legal_moves();
        benchmark_make_move();
        benchmark_zobrist_hash();
        benchmark_memory_operations();
        benchmark_position_evaluation();
        
        // Quality tests
        test_hash_quality();
        
        // Summary
        compare_with_baseline();
        
        std::cout << "\n";
        std::cout << "+============================================================+\n";
        std::cout << "|  Benchmark Complete!                                       |\n";
        std::cout << "|  All performance metrics recorded.                         |\n";
        std::cout << "+============================================================+\n";
        std::cout << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

