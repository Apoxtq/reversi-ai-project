/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * AI Strategy Interface
 */

#pragma once

#include "../core/Board.hpp"
#include "../core/Move.hpp"
#include <string>
#include <memory>

namespace reversi {
namespace ai {

/**
 * @brief Search limits for AI
 */
struct SearchLimits {
    int max_depth = 6;        // Maximum search depth
    int max_time_ms = 5000;   // Maximum time in milliseconds
    uint64_t max_nodes = 0;   // Maximum nodes (0 = unlimited)
    
    SearchLimits() = default;
    SearchLimits(int depth, int time_ms) 
        : max_depth(depth), max_time_ms(time_ms) {}
};

/**
 * @brief Search statistics
 */
struct SearchStats {
    uint64_t nodes_searched = 0;
    int depth_reached = 0;
    int time_elapsed_ms = 0;
    double nodes_per_second = 0.0;
    
    virtual ~SearchStats() = default;
    
    virtual void reset() {
        nodes_searched = 0;
        depth_reached = 0;
        time_elapsed_ms = 0;
        nodes_per_second = 0.0;
    }
};

/**
 * @brief Abstract interface for AI strategies
 * 
 * All AI algorithms (Minimax, MCTS, etc.) must implement this interface.
 */
class AIStrategy {
public:
    virtual ~AIStrategy() = default;
    
    /**
     * @brief Find the best move for the given board position
     * 
     * @param board Current board state
     * @param limits Search limits (depth, time)
     * @return Best move found
     */
    virtual core::Move find_best_move(const core::Board& board, 
                                      const SearchLimits& limits) = 0;
    
    /**
     * @brief Get the name of this AI strategy
     */
    virtual std::string get_name() const = 0;
    
    /**
     * @brief Get search statistics from last move
     */
    virtual const SearchStats& get_stats() const = 0;
    
    /**
     * @brief Reset internal state (transposition table, etc.)
     */
    virtual void reset() = 0;
};

/**
 * @brief Difficulty presets
 */
enum class Difficulty {
    EASY,    // Depth 2-3, 1 second
    MEDIUM,  // Depth 4-5, 3 seconds
    HARD     // Depth 6-8, 5 seconds
};

/**
 * @brief Create SearchLimits from difficulty preset
 */
inline SearchLimits get_limits_from_difficulty(Difficulty diff) {
    switch (diff) {
        case Difficulty::EASY:
            return SearchLimits(3, 1000);
        case Difficulty::MEDIUM:
            return SearchLimits(5, 3000);
        case Difficulty::HARD:
            return SearchLimits(7, 5000);
        default:
            return SearchLimits(4, 2000);
    }
}

} // namespace ai
} // namespace reversi

