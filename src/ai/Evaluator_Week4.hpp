/*
 * Evaluator_Week4.hpp - Enhanced position evaluation (Week 4)
 * COMP390 Honours Year Project
 * 
 * Week 4 enhancements:
 * - Stability analysis (stable/frontier discs)
 * - Phase-aware weight system (4 phases)
 * - Potential mobility calculation
 * - Dynamic corner control evaluation
 * 
 * This is a new version - old Evaluator.hpp preserved for comparison
 */

#pragma once

#include "core/Board.hpp"
#include "ai/StabilityAnalyzer.hpp"
#include "ai/PhaseWeights.hpp"
#include <cstdint>

namespace reversi::ai {

/**
 * @brief Enhanced position evaluator with Week 4 features
 * 
 * Evaluation components:
 * 1. Material (piece count) - phase-weighted
 * 2. Mobility (legal moves) - phase-weighted
 * 3. Potential mobility (future options) - opening/midgame
 * 4. Stability (stable discs) - midgame/endgame
 * 5. Frontier penalty (weak discs) - opening/midgame
 * 6. Position weights (corners/edges) - phase-weighted
 * 7. Corner control bonus - all phases
 */
class EvaluatorWeek4 {
public:
    /**
     * @brief Evaluate position with enhanced features
     * 
     * @param board Board to evaluate
     * @return Score from current player's perspective
     */
    static int evaluate(const reversi::core::Board& board) noexcept;
    
    /**
     * @brief Fast evaluation for deep nodes (simplified)
     * 
     * Skips expensive calculations like full stability analysis.
     * Use when depth > 6 where speed matters more than accuracy.
     * 
     * @param board Board to evaluate
     * @return Score (less accurate but faster)
     */
    static int evaluate_fast(const reversi::core::Board& board) noexcept;
    
private:
    /**
     * @brief Calculate piece count score (material)
     */
    static inline int piece_count_score(const reversi::core::Board& board) noexcept {
        return board.count_player() - board.count_opponent();
    }
    
    /**
     * @brief Calculate mobility score (current legal moves)
     */
    static int mobility_score(const reversi::core::Board& board) noexcept;
    
    /**
     * @brief Calculate potential mobility (empty squares adjacent to opponent)
     * 
     * High potential mobility = more future options
     */
    static int potential_mobility_score(const reversi::core::Board& board) noexcept;
    
    /**
     * @brief Calculate position weight score with phase scaling
     */
    static int position_score(const reversi::core::Board& board, int scale) noexcept;
    
    /**
     * @brief Calculate corner control bonus
     * 
     * Beyond static position weights, corners get extra bonus
     * for their strategic importance (stability propagation)
     */
    static int corner_control_score(const reversi::core::Board& board) noexcept;
    
    /**
     * @brief Position weight table (same as Week 3)
     */
    static constexpr int POSITION_WEIGHTS[64] = {
        100, -20,  10,   5,   5,  10, -20, 100,
        -20, -50,  -2,  -2,  -2,  -2, -50, -20,
         10,  -2,   1,   1,   1,   1,  -2,  10,
          5,  -2,   1,   0,   0,   1,  -2,   5,
          5,  -2,   1,   0,   0,   1,  -2,   5,
         10,  -2,   1,   1,   1,   1,  -2,  10,
        -20, -50,  -2,  -2,  -2,  -2, -50, -20,
        100, -20,  10,   5,   5,  10, -20, 100
    };
};

} // namespace reversi::ai

