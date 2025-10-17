/*
 * Hash cache vs recompute consistency test
 */

#include "core/Board.hpp"
#include <cassert>
#include <random>
#include <bit>

using namespace reversi::core;

static uint64_t recompute_reference(const Board& b) {
    uint64_t p = b.player;
    uint64_t o = b.opponent;
    uint64_t h = 0;
    while (p) {
        int pos = std::countr_zero(p);
        h ^= Board::zobrist_player[pos];
        p &= p - 1;
    }
    while (o) {
        int pos = std::countr_zero(o);
        h ^= Board::zobrist_opponent[pos];
        o &= o - 1;
    }
    return h;
}

int main() {
    Board b;
    std::mt19937 rng(42);

    for (int step = 0; step < 200; ++step) {
        auto moves = b.get_legal_moves();
        if (moves.empty()) {
            b.pass();
            continue;
        }
        int idx = rng() % moves.size();
        b.make_move(moves[idx]);

        uint64_t h_cache = b.hash();
        uint64_t h_ref = recompute_reference(b);
        assert(h_cache == h_ref);

        // Occasionally undo
        if (step % 7 == 0) {
            b.undo_move(0);
            assert(b.hash() == recompute_reference(b));
        }
    }
    return 0;
}


