/*
 * Evaluator.cpp - Implementation of position evaluation
 * COMP390 Honours Year Project
 */

#include "ai/Evaluator.hpp"
#include <bit>

namespace reversi::ai {

int Evaluator::evaluate(const reversi::core::Board& board) noexcept {
    // Terminal state: return definitive score
    if (board.is_terminal()) {
        const int diff = board.count_player() - board.count_opponent();
        if (diff > 0) return 10000;      // Win
        if (diff < 0) return -10000;     // Loss
        return 0;                         // Draw
    }
    
    int score = 0;
    
    // 1. Piece count (more important in endgame)
    const int total_pieces = board.count_player() + board.count_opponent();
    if (total_pieces > 50) {
        // Endgame (50+ pieces): piece count matters a lot
        score += piece_count_score(board) * 10;
    } else {
        // Opening/midgame: piece count is less important
        score += piece_count_score(board);
    }
    
    // 2. Position weights (corners, edges, dangerous squares)
    score += position_score(board);
    
    // 3. Mobility (ability to move)
    score += mobility_score(board) * 5;
    
    return score;
}

int Evaluator::piece_count_score(const reversi::core::Board& board) noexcept {
    return board.count_player() - board.count_opponent();
}

int Evaluator::position_score(const reversi::core::Board& board) noexcept {
    int score = 0;
    
    // Get bitboards
    uint64_t player_bb = board.player;
    uint64_t opponent_bb = board.opponent;
    
    // Iterate through all 64 positions
    for (int pos = 0; pos < 64; ++pos) {
        uint64_t mask = 1ULL << pos;
        if (player_bb & mask) {
            score += POSITION_WEIGHTS[pos];
        } else if (opponent_bb & mask) {
            score -= POSITION_WEIGHTS[pos];
        }
    }
    
    return score;
}

int Evaluator::mobility_score(const reversi::core::Board& board) noexcept {
    // Current player's mobility
    const int player_moves = static_cast<int>(board.get_legal_moves().size());
    
    // Opponent's mobility (after pass)
    reversi::core::Board opp_board = board;
    opp_board.pass();
    const int opp_moves = static_cast<int>(opp_board.get_legal_moves().size());
    
    return player_moves - opp_moves;
}

} // namespace reversi::ai

