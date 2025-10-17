/*
 * Undo move unit test
 */

#include "core/Board.hpp"
#include <cassert>

using namespace reversi::core;

int main() {
    Board b;
    auto moves = b.get_legal_moves();
    assert(moves.size() == 4);

    const int pos = moves[0];
    const int p_before = b.count_player();
    const int o_before = b.count_opponent();

    b.make_move(pos);

    // After a valid move, counts must change and side swaps
    const int p_after = b.count_player();
    const int o_after = b.count_opponent();
    assert(p_after != p_before || o_after != o_before);

    // Now undo should return to exact previous board state (including side to move)
    b.undo_move(pos);
    assert(b.count_player() == p_before);
    assert(b.count_opponent() == o_before);

    // Legal moves from initial position should be 4 again
    auto moves2 = b.get_legal_moves();
    assert(moves2.size() == 4);
    return 0;
}


