/*
 * Evaluator_Week4.cpp - Implementation of enhanced evaluation
 * COMP390 Honours Year Project - Week 4
 */

#include "ai/Evaluator_Week4.hpp"
#include <bit>

namespace reversi::ai {

int EvaluatorWeek4::evaluate(const reversi::core::Board& board) noexcept {
    // Terminal state: return definitive score
    if (board.is_terminal()) {
        const int diff = board.count_player() - board.count_opponent();
        if (diff > 0) return 10000;
        if (diff < 0) return -10000;
        return 0;
    }
    
    // Detect game phase
    const auto weights = PhaseWeights::get_weights(board);
    
    int score = 0;
    
    // 1. Material (piece count)
    score += piece_count_score(board) * weights.piece_count_weight;
    
    // 2. Mobility (legal moves)
    score += mobility_score(board) * weights.mobility_weight;
    
    // 3. Potential mobility (future options)
    if (weights.potential_mobility_weight > 0) {
        score += potential_mobility_score(board) * weights.potential_mobility_weight;
    }
    
    // 4. Stability analysis
    if (weights.stability_weight > 0) {
        const auto stability = StabilityAnalyzer::analyze(board);
        score += stability.stable_count * weights.stability_weight;
    }
    
    // 5. Frontier penalty
    if (weights.frontier_penalty != 0) {
        const auto stability = StabilityAnalyzer::analyze(board);
        score += stability.frontier_count * weights.frontier_penalty;
    }
    
    // 6. Position weights (scaled by phase)
    score += position_score(board, weights.position_weight_scale);
    
    // 7. Corner control bonus
    score += corner_control_score(board) * weights.corner_bonus;
    
    return score;
}

int EvaluatorWeek4::evaluate_fast(const reversi::core::Board& board) noexcept {
    // Terminal state
    if (board.is_terminal()) {
        const int diff = board.count_player() - board.count_opponent();
        if (diff > 0) return 10000;
        if (diff < 0) return -10000;
        return 0;
    }
    
    // Simplified evaluation for deep nodes
    const auto weights = PhaseWeights::get_weights(board);
    
    int score = 0;
    
    // Only use fast components
    score += piece_count_score(board) * weights.piece_count_weight;
    score += mobility_score(board) * weights.mobility_weight;
    score += position_score(board, weights.position_weight_scale);
    
    // Fast corner stability (not full stability)
    score += StabilityAnalyzer::calculate_corner_stability(board);
    
    return score;
}

int EvaluatorWeek4::mobility_score(const reversi::core::Board& board) noexcept {
    // Current player's mobility
    const int player_moves = static_cast<int>(board.get_legal_moves().size());
    
    // Opponent's mobility (after pass)
    reversi::core::Board opp_board = board;
    opp_board.pass();
    const int opp_moves = static_cast<int>(opp_board.get_legal_moves().size());
    
    return player_moves - opp_moves;
}

int EvaluatorWeek4::potential_mobility_score(const reversi::core::Board& board) noexcept {
    // Empty squares
    uint64_t empty = ~(board.player | board.opponent);
    
    // Shift empty in all 8 directions to find adjacent pieces
    auto shift_all = [](uint64_t bb) -> uint64_t {
        uint64_t result = 0;
        result |= (bb << 8);  // N
        result |= (bb >> 8);  // S
        result |= ((bb << 1) & 0xFEFEFEFEFEFEFEFEULL);  // E
        result |= ((bb >> 1) & 0x7F7F7F7F7F7F7F7FULL);  // W
        result |= ((bb << 9) & 0xFEFEFEFEFEFEFEFEULL);  // NE
        result |= ((bb << 7) & 0x7F7F7F7F7F7F7F7FULL);  // NW
        result |= ((bb >> 7) & 0xFEFEFEFEFEFEFEFEULL);  // SE
        result |= ((bb >> 9) & 0x7F7F7F7F7F7F7F7FULL);  // SW
        return result;
    };
    
    // Empty squares adjacent to opponent = player's potential
    uint64_t player_potential = shift_all(empty) & board.opponent;
    int player_count = std::popcount(player_potential);
    
    // Empty squares adjacent to player = opponent's potential
    uint64_t opp_potential = shift_all(empty) & board.player;
    int opp_count = std::popcount(opp_potential);
    
    return player_count - opp_count;
}

int EvaluatorWeek4::position_score(const reversi::core::Board& board, int scale) noexcept {
    int score = 0;
    
    uint64_t player_bb = board.player;
    uint64_t opponent_bb = board.opponent;
    
    for (int pos = 0; pos < 64; ++pos) {
        uint64_t mask = 1ULL << pos;
        if (player_bb & mask) {
            score += POSITION_WEIGHTS[pos];
        } else if (opponent_bb & mask) {
            score -= POSITION_WEIGHTS[pos];
        }
    }
    
    return score * scale;
}

int EvaluatorWeek4::corner_control_score(const reversi::core::Board& board) noexcept {
    // Corner positions
    constexpr uint64_t CORNERS = 
        0x0000000000000001ULL |  // A1
        0x0000000000000080ULL |  // H1
        0x0100000000000000ULL |  // A8
        0x8000000000000000ULL;   // H8
    
    int player_corners = std::popcount(board.player & CORNERS);
    int opp_corners = std::popcount(board.opponent & CORNERS);
    
    // Return multiplier (will be multiplied by corner_bonus weight)
    return player_corners - opp_corners;
}

} // namespace reversi::ai

