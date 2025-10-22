/*
 * Evaluator.hpp - Position evaluation for Reversi
 * COMP390 Honours Year Project
 * 
 * Static position evaluator with multiple evaluation components:
 * - Piece count (material advantage)
 * - Position weights (corners, edges, etc.)
 * - Mobility (number of legal moves)
 * - Stability (future-proof positions)
 * 
 * Performance target: > 50M evaluations/second
 */

#pragma once

#include "core/Board.hpp"
#include <cstdint>

namespace reversi::ai {

/**
 * @brief Static position evaluator for Reversi
 * 
 * Evaluates board positions using a weighted combination of features:
 * 1. Piece count - more important in endgame
 * 2. Position weights - corners are most valuable
 * 3. Mobility - ability to make moves
 * 4. Stability - pieces that cannot be flipped (future enhancement)
 */
class Evaluator {
public:
    /**
     * @brief Evaluate a board position from current player's perspective
     * 
     * @param board The board to evaluate
     * @return Score in centipawns (positive = good for current player)
     * 
     * Terminal positions return large scores:
     *   +10000 for win, -10000 for loss, 0 for draw
     * 
     * Performance: inline, noexcept, > 50M evals/sec
     */
    static int evaluate(const reversi::core::Board& board) noexcept;
    
private:
    /**
     * @brief Position weight table
     * 
     * Corners (100): Most valuable, cannot be flipped once captured
     * X-squares (-50): Adjacent to corners, dangerous early game
     * C-squares (-20): Adjacent to corners, often give away corners
     * Edges (10): Relatively stable
     * Center (0-5): Flexible but less stable
     */
    static constexpr int POSITION_WEIGHTS[64] = {
        100, -20,  10,   5,   5,  10, -20, 100,  // Row 0
        -20, -50,  -2,  -2,  -2,  -2, -50, -20,  // Row 1
         10,  -2,   1,   1,   1,   1,  -2,  10,  // Row 2
          5,  -2,   1,   0,   0,   1,  -2,   5,  // Row 3
          5,  -2,   1,   0,   0,   1,  -2,   5,  // Row 4
         10,  -2,   1,   1,   1,   1,  -2,  10,  // Row 5
        -20, -50,  -2,  -2,  -2,  -2, -50, -20,  // Row 6
        100, -20,  10,   5,   5,  10, -20, 100   // Row 7
    };
    
    /**
     * @brief Evaluate piece count difference
     * @return Player pieces - opponent pieces
     */
    static inline int piece_count_score(const reversi::core::Board& board) noexcept;
    
    /**
     * @brief Evaluate position weights
     * @return Weighted sum of piece positions
     */
    static inline int position_score(const reversi::core::Board& board) noexcept;
    
    /**
     * @brief Evaluate mobility (number of legal moves)
     * @return Player mobility - opponent mobility
     */
    static inline int mobility_score(const reversi::core::Board& board) noexcept;
};

} // namespace reversi::ai

