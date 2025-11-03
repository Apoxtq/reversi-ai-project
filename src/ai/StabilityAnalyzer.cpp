/*
 * StabilityAnalyzer.cpp - Implementation of stability analysis
 * COMP390 Honours Year Project - Week 4
 */

#include "ai/StabilityAnalyzer.hpp"
#include <bit>

namespace reversi::ai {

StabilityResult StabilityAnalyzer::analyze(const reversi::core::Board& board) noexcept {
    StabilityResult result = {0, 0, 0, 0ULL, 0ULL};
    
    // 1. Calculate corner-based stability
    uint64_t player_stable = 0ULL;
    uint64_t opponent_stable = 0ULL;
    
    // Check all 4 corners
    const int corners[4] = {0, 7, 56, 63};  // A1, H1, A8, H8
    
    for (int corner : corners) {
        // Check if corner captured by player
        if (board.player & (1ULL << corner)) {
            player_stable |= (1ULL << corner);
            // Propagate stability from this corner
            // TODO: Implement propagate_edge_stability
        }
        
        // Check if corner captured by opponent
        if (board.opponent & (1ULL << corner)) {
            opponent_stable |= (1ULL << corner);
            // Propagate stability from this corner
            // TODO: Implement propagate_edge_stability
        }
    }
    
    // 2. Count stable pieces
    result.player_stable = player_stable;
    result.opponent_stable = opponent_stable;
    result.stable_count = std::popcount(player_stable) - std::popcount(opponent_stable);
    
    // 3. Calculate frontier discs
    result.frontier_count = calculate_frontier(board);
    
    // 4. Semi-stable pieces (for future enhancement)
    result.semi_stable_count = 0;  // TODO: Implement semi-stability
    
    return result;
}

int StabilityAnalyzer::calculate_corner_stability(const reversi::core::Board& board) noexcept {
    int score = 0;
    
    // Simple corner counting with wedge expansion
    const int corners[4] = {0, 7, 56, 63};
    
    for (int corner : corners) {
        uint64_t corner_mask = 1ULL << corner;
        
        if (board.player & corner_mask) {
            // Player owns corner: +100 base score
            score += 100;
            
            // Bonus for stable wedge (adjacent edge pieces)
            score += count_stable_from_corner(board, corner, board.player) * 10;
        }
        else if (board.opponent & corner_mask) {
            // Opponent owns corner: -100 base score
            score -= 100;
            
            // Penalty for opponent's stable wedge
            score -= count_stable_from_corner(board, corner, board.opponent) * 10;
        }
    }
    
    return score;
}

int StabilityAnalyzer::calculate_frontier(const reversi::core::Board& board) noexcept {
    // Get empty squares
    uint64_t empty = ~(board.player | board.opponent);
    
    // Count player's frontier discs
    int player_frontier = count_adjacent_to_empty(board.player, empty);
    
    // Count opponent's frontier discs
    int opponent_frontier = count_adjacent_to_empty(board.opponent, empty);
    
    // Return difference (negative is good - fewer frontier discs)
    return player_frontier - opponent_frontier;
}

int StabilityAnalyzer::count_stable_from_corner(
    const reversi::core::Board& board,
    int corner,
    uint64_t player_pieces) noexcept
{
    int stable_count = 0;
    
    // Define directions to check based on corner
    // A1 (0): check right (+1) and up (+8)
    // H1 (7): check left (-1) and up (+8)
    // A8 (56): check right (+1) and down (-8)
    // H8 (63): check left (-1) and down (-8)
    
    int dir1 = 0, dir2 = 0;
    
    switch (corner) {
        case 0:  // A1
            dir1 = 1;   // right
            dir2 = 8;   // up
            break;
        case 7:  // H1
            dir1 = -1;  // left
            dir2 = 8;   // up
            break;
        case 56: // A8
            dir1 = 1;   // right
            dir2 = -8;  // down
            break;
        case 63: // H8
            dir1 = -1;  // left
            dir2 = -8;  // down
            break;
    }
    
    // Count stable pieces along first edge (row or column)
    int pos = corner + dir1;
    while (pos >= 0 && pos < 64) {
        // Check if still on same row/column
        if ((corner % 8 == 0 || corner % 8 == 7) && (pos / 8) != (corner / 8)) {
            break;  // Moved to different row
        }
        if ((corner / 8 == 0 || corner / 8 == 7) && (pos % 8) != (corner % 8)) {
            break;  // Moved to different column
        }
        
        if (player_pieces & (1ULL << pos)) {
            stable_count++;
            pos += dir1;
        } else {
            break;  // Gap or opponent piece
        }
    }
    
    // Count stable pieces along second edge
    pos = corner + dir2;
    while (pos >= 0 && pos < 64) {
        if (player_pieces & (1ULL << pos)) {
            stable_count++;
            pos += dir2;
        } else {
            break;  // Gap or opponent piece
        }
    }
    
    return stable_count;
}

void StabilityAnalyzer::propagate_edge_stability(
    uint64_t pieces,
    uint64_t& stable,
    int start,
    int direction,
    int limit) noexcept
{
    // TODO: Implement edge stability propagation
    // This will be completed in Day 2
    (void)pieces;
    (void)stable;
    (void)start;
    (void)direction;
    (void)limit;
}

uint64_t StabilityAnalyzer::calculate_interior_stability(
    const reversi::core::Board& board,
    uint64_t edge_stable) noexcept
{
    // TODO: Implement interior stability analysis
    // This is an advanced feature for later optimization
    (void)board;
    (void)edge_stable;
    return 0ULL;
}

int StabilityAnalyzer::count_adjacent_to_empty(uint64_t pieces, uint64_t empty) noexcept {
    // Shift empty squares in all 8 directions
    uint64_t adjacent_to_empty = 
        shift_N(empty) | shift_S(empty) |
        shift_E(empty) | shift_W(empty) |
        shift_NE(empty) | shift_NW(empty) |
        shift_SE(empty) | shift_SW(empty);
    
    // Count pieces that are adjacent to empty
    return std::popcount(pieces & adjacent_to_empty);
}

} // namespace reversi::ai

