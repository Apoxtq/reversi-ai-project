/*
 * MinimaxEngine.hpp - Minimax search with Alpha-Beta pruning
 * COMP390 Honours Year Project
 * 
 * Classical minimax search engine implementing:
 * - Negamax framework (simplified minimax)
 * - Alpha-Beta pruning (performance optimization)
 * - Configurable search depth
 * - Detailed search statistics
 * 
 * Performance target: > 2M nodes/second at depth 6
 */

#pragma once

#include "core/Board.hpp"
#include "ai/Evaluator.hpp"
#include <limits>
#include <chrono>
#include <cstdint>

namespace reversi::ai {

/**
 * @brief Minimax search engine with Alpha-Beta pruning
 * 
 * Uses Negamax framework which simplifies implementation:
 * - Single recursive function instead of separate min/max
 * - Scores are negated when passing to opponent
 * - Alpha-beta bounds are negated and swapped
 */
class MinimaxEngine {
public:
    /**
     * @brief Search configuration
     */
    struct Config {
        int max_depth = 6;           ///< Maximum search depth (plies)
        bool use_alpha_beta = true;  ///< Enable Alpha-Beta pruning
        
        Config() = default;
        Config(int depth, bool ab = true) 
            : max_depth(depth), use_alpha_beta(ab) {}
    };
    
    /**
     * @brief Search result with complete statistics
     */
    struct SearchResult {
        int best_move = -1;         ///< Best move found (0-63, or -1 for pass)
        int score = 0;              ///< Evaluation score for best move
        int nodes_searched = 0;     ///< Total nodes explored
        int depth_reached = 0;      ///< Actual depth searched
        double time_ms = 0.0;       ///< Search time in milliseconds
        
        /**
         * @brief Calculate search speed
         * @return Nodes per second
         */
        double nodes_per_sec() const {
            return time_ms > 0 ? (nodes_searched * 1000.0 / time_ms) : 0.0;
        }
        
        /**
         * @brief Print search statistics
         */
        void print() const;
    };
    
    /**
     * @brief Construct engine with default configuration
     */
    MinimaxEngine();
    
    /**
     * @brief Construct engine with custom configuration
     */
    explicit MinimaxEngine(const Config& config);
    
    /**
     * @brief Find best move for current board position
     * 
     * @param board Board position to search
     * @return SearchResult containing best move and statistics
     * 
     * Special cases:
     * - No legal moves: returns best_move = -1 (pass)
     * - Only one legal move: returns immediately without search
     */
    SearchResult find_best_move(const reversi::core::Board& board);
    
    /**
     * @brief Get current configuration
     */
    const Config& config() const { return config_; }
    
    /**
     * @brief Set new configuration
     */
    void set_config(const Config& config) { config_ = config; }
    
private:
    Config config_;           ///< Search configuration
    int nodes_searched_ = 0;  ///< Node counter (reset each search)
    
    /**
     * @brief Negamax search with alpha-beta pruning
     * 
     * @param board Current board position (modified during search)
     * @param depth Remaining depth to search
     * @param alpha Lower bound (best score for current player so far)
     * @param beta Upper bound (best score for opponent so far)
     * @return Best score for current player
     * 
     * Pruning condition: if alpha >= beta, opponent won't allow this path
     * 
     * Performance: inline-able hot path, noexcept for performance
     */
    int negamax(reversi::core::Board& board, int depth, int alpha, int beta);
};

} // namespace reversi::ai

