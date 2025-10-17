/*
 * Micro-benchmarks for core board operations
 * - legal_moves()
 * - make_move()/undo_move() pair
 * - pass()
 *
 * Notes:
 * - Targets quick runtime (< 5s) while giving stable numbers.
 * - Build with -O3 -march=native -DNDEBUG for meaningful results.
 */

#include "core/Board.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using reversi::core::Board;

namespace {

using Clock = std::chrono::high_resolution_clock;

struct BenchResult {
    const char* name;
    double opsPerSec;
    double durationMs;
    uint64_t iterations;
};

template <class F>
BenchResult run_fixed_time(const char* name, F&& func, double target_seconds = 0.8) {
    const auto start = Clock::now();
    const auto deadline = start + std::chrono::duration<double>(target_seconds);
    uint64_t iters = 0;

    // Small warmup
    for (int i = 0; i < 1000; ++i) func();

    while (Clock::now() < deadline) {
        func();
        ++iters;
    }

    const auto end = Clock::now();
    const double ms = std::chrono::duration<double, std::milli>(end - start).count();
    const double ops_per_sec = (iters * 1000.0) / ms;
    return BenchResult{name, ops_per_sec, ms, iters};
}

void print_result(const BenchResult& r) {
    std::cout << r.name << ": " << r.opsPerSec << " ops/sec"
              << " (" << r.iterations << " iters in " << r.durationMs << " ms)\n";
}

} // namespace

int main() {
    std::mt19937 rng(12345);
    Board board; // starts from initial position

    // Pre-generate a sequence of random moves to reduce RNG cost in timed loops
    std::vector<int> move_buffer;
    move_buffer.reserve(64);

    // 1) legal_moves() benchmark
    volatile uint64_t sink_moves = 0; // prevent over-optimization
    auto bench_legal_moves = [&]() {
        sink_moves ^= board.legal_moves();
        // Randomly perturb board occasionally to avoid being stuck at initial state
        if ((rng() & 0xFF) == 0) {
            auto moves = board.get_legal_moves();
            if (!moves.empty()) {
                board.make_move(moves[static_cast<size_t>(rng() % moves.size())]);
            } else {
                board.pass();
            }
        }
    };

    const auto r1 = run_fixed_time("legal_moves", bench_legal_moves);
    print_result(r1);

    // Reset board
    board = Board();

    // 2) make_move + undo_move pair benchmark (keeps state bounded)
    auto bench_make_undo = [&]() {
        auto moves = board.get_legal_moves();
        if (!moves.empty()) {
            const int pos = moves[static_cast<size_t>(rng() % moves.size())];
            board.make_move(pos);
            board.undo_move(pos);
        } else {
            board.pass();
            board.pass(); // restore to original side if needed
        }
    };

    const auto r2 = run_fixed_time("make_move+undo", bench_make_undo);
    print_result(r2);

    // 3) pass() benchmark (includes hash recomputation in current implementation)
    auto bench_pass = [&]() {
        board.pass();
        board.pass(); // return to original side so board doesn't diverge
    };
    const auto r3 = run_fixed_time("pass_pair", bench_pass);
    print_result(r3);

    // A simple overall indicator: random playout throughput to terminal or fixed steps
    board = Board();
    auto bench_playout = [&]() {
        auto moves = board.get_legal_moves();
        if (!moves.empty()) {
            const int pos = moves[static_cast<size_t>(rng() % moves.size())];
            board.make_move(pos);
        } else {
            board.pass();
        }
        // If terminal, reset to keep running
        if (board.is_terminal()) {
            board = Board();
        }
    };
    const auto r4 = run_fixed_time("random_playout_steps", bench_playout, 1.2);
    print_result(r4);

    // Structured summary (machine-friendly)
    std::cout << "SUMMARY legal_moves=" << r1.opsPerSec
              << " make_undo=" << r2.opsPerSec
              << " pass_pair=" << r3.opsPerSec
              << " playout_steps=" << r4.opsPerSec << "\n";

    // Prevent dead-code elimination on sink_moves
    if (sink_moves == 0xDEADBEEF) std::cerr << "";
    return 0;
}


