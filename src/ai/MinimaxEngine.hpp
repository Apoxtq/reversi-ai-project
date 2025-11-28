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
#include "core/Move.hpp"
#include "ai/Evaluator.hpp"
#include "ai/TranspositionTable.hpp"
#include "ai/AIStrategy.hpp"
#include <limits>
#include <chrono>
#include <cstdint>
#include <array>
#include <vector>
#include <algorithm>
#include <string>

namespace reversi::ai {

/**
 * @brief Minimax search engine with Alpha-Beta pruning
 * 
 * Uses Negamax framework which simplifies implementation:
 * - Single recursive function instead of separate min/max
 * - Scores are negated when passing to opponent
 * - Alpha-beta bounds are negated and swapped
 * 
 * Week 6 enhancements:
 * - Iterative deepening search with time management
 * - Principal Variation Search (PVS)
 * - Aspiration windows
 * - Killer move heuristics
 * 
 * Implements AIStrategy interface for integration with game system.
 */
class MinimaxEngine : public AIStrategy {
public:
    /**
     * @brief Search configuration
     */
    struct Config {
        int max_depth = 6;           ///< Maximum search depth (plies)
        bool use_alpha_beta = true;  ///< Enable Alpha-Beta pruning
        bool use_transposition = true; ///< Enable transposition table
        int tt_size_bits = 20;       ///< TT size = 2^size_bits (default: 1M entries)
        
        // Week 6: Advanced search optimizations
        int time_limit_ms = 0;       ///< Time limit in milliseconds (0 = no limit)
        bool use_iterative_deepening = false; ///< Enable iterative deepening
        bool use_pvs = false;        ///< Enable Principal Variation Search
        bool use_aspiration = false; ///< Enable aspiration windows
        bool use_killer_moves = false; ///< Enable killer move heuristics
        int aspiration_window = 50;  ///< Aspiration window size (centipawns)
        
        Config() = default;
        Config(int depth, bool ab = true, bool tt = true, int tt_bits = 20) 
            : max_depth(depth), use_alpha_beta(ab), use_transposition(tt), tt_size_bits(tt_bits) {}
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
     * 
     * If iterative deepening is enabled, searches from depth 1 to max_depth
     * until time limit is reached or max_depth is achieved.
     */
    SearchResult find_best_move(const reversi::core::Board& board);
    
    // AIStrategy interface implementation
    /**
     * @brief Find best move (AIStrategy interface)
     * 
     * @param board Current board state
     * @param limits Search limits (depth, time)
     * @return Best move found
     */
    reversi::core::Move find_best_move(const reversi::core::Board& board, 
                                       const SearchLimits& limits) override;
    
    /**
     * @brief Get the name of this AI strategy
     */
    std::string get_name() const override {
        return "Minimax (Negamax + Alpha-Beta)";
    }
    
    /**
     * @brief Get search statistics from last move
     */
    const SearchStats& get_stats() const override {
        return last_stats_;
    }
    
    /**
     * @brief Reset internal state (transposition table, etc.)
     */
    void reset() override {
        clear_tt();
        clear_killers();
        last_stats_.reset();
    }
    
    /**
     * @brief Get current configuration
     */
    const Config& config() const { return config_; }
    
    /**
     * @brief Set new configuration
     */
    void set_config(const Config& config) { config_ = config; }
    
    /**
     * @brief Get transposition table statistics
     */
    TranspositionTable::Stats get_tt_stats() const {
        return tt_.get_stats();
    }
    
    /**
     * @brief Clear transposition table
     */
    void clear_tt() {
        tt_.clear();
    }
    
private:
    // Constants
    static constexpr int MAX_DEPTH = 64;  ///< Maximum search depth (game has 64 squares)
    static constexpr int TIME_CHECK_INTERVAL = 1000;  ///< Check time every N nodes
    
    Config config_;           ///< Search configuration
    TranspositionTable tt_;   ///< Transposition table
    int nodes_searched_ = 0;  ///< Node counter (reset each search)
    
    // AIStrategy interface: last search statistics
    mutable SearchStats last_stats_;  ///< Statistics from last search
    
    // Week 6: Time management
    std::chrono::high_resolution_clock::time_point search_start_;
    int time_limit_ms_ = 0;
    bool time_exceeded_ = false;
    
    // Week 6: Killer moves (per ply)
    std::array<int, MAX_DEPTH> killer1_;  ///< First killer move at each ply
    std::array<int, MAX_DEPTH> killer2_;  ///< Second killer move at each ply
    int current_ply_ = 0;  ///< Current search ply (for killer moves)
    
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
    
    /**
     * @brief Principal Variation Search (PVS) / NegaScout
     * 
     * First move uses full window, subsequent moves use zero window.
     * If zero window fails, re-search with full window.
     * 
     * @param board Current board position
     * @param depth Remaining depth
     * @param alpha Lower bound
     * @param beta Upper bound
     * @param is_pv Whether this is a principal variation node
     * @return Best score for current player
     */
    int pvs(reversi::core::Board& board, int depth, int alpha, int beta, bool is_pv);
    
    /**
     * @brief Iterative deepening search
     * 
     * Searches from depth 1 to max_depth, using transposition table
     * to accelerate deeper searches.
     * 
     * @param board Board position to search
     * @return SearchResult with best move found
     */
    SearchResult iterative_deepening_search(const reversi::core::Board& board);
    
    /**
     * @brief Aspiration window search
     * 
     * Uses a narrow window around predicted score. If search fails,
     * re-searches with full window.
     * 
     * @param board Board position to search
     * @param depth Search depth
     * @param predicted_score Predicted score from transposition table
     * @return SearchResult with best move
     */
    SearchResult aspiration_search(const reversi::core::Board& board, int depth, int predicted_score);
    
    /**
     * @brief Calculate time limit based on game phase
     * 
     * Allocates more time in endgame, less in opening.
     * 
     * @param board Current board position
     * @param total_time_ms Total time available
     * @return Time limit for this move
     */
    int calculate_time_limit(const reversi::core::Board& board, int total_time_ms) const;
    
    /**
     * @brief Check if time limit has been exceeded
     * 
     * @return true if time exceeded, false otherwise
     */
    bool time_exceeded() const;
    
    /**
     * @brief Update killer move table
     * 
     * @param move Move that caused beta cutoff
     * @param ply Current search ply
     */
    void update_killer(int move, int ply);
    
    /**
     * @brief Get killer move bonus for move ordering
     * 
     * @param move Move to check
     * @param ply Current search ply
     * @return Bonus score (higher = better)
     */
    int get_killer_bonus(int move, int ply) const;
    
    /**
     * @brief Clear killer move table
     */
    void clear_killers();
    
    /**
     * @brief Order moves with heuristics (TT, killer, evaluation)
     * 
     * @param board Current board position
     * @param moves Legal moves to order
     * @return Ordered moves (best first)
     */
    std::vector<int> order_moves(const reversi::core::Board& board, const std::vector<int>& moves) const;
};

} // namespace reversi::ai

