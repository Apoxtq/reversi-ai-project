/*
 * MatchEngine.hpp - AI vs AI Match Engine
 * COMP390 Honours Year Project - Week 10
 * 
 * High-quality match engine for AI vs AI comparisons
 * Supports color alternation, statistics collection, and best-of-N series
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#pragma once

#include "../../ai/AIStrategy.hpp"
#include "../../core/Board.hpp"
#include "../../core/Move.hpp"
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <cstdint>

namespace reversi {
namespace research {

/**
 * @brief AI vs AI match engine
 * 
 * Provides comprehensive match execution with:
 * - Single game and series support
 * - Color alternation (eliminates first-move advantage)
 * - Detailed statistics collection
 * - Performance metrics tracking
 */
class MatchEngine {
public:
    /**
     * @brief Match configuration
     */
    struct MatchConfig {
        int num_games = 10;              ///< Total number of games
        bool alternate_colors = true;    ///< Alternate colors between games
        bool best_of_series = false;     ///< Use best-of-N series mode
        int series_length = 3;           ///< Series length (best-of-3, etc.)
        uint32_t random_seed = 0;        ///< Random seed (0 = use time)
        bool verbose = false;            ///< Verbose output
        bool collect_move_history = false; ///< Collect detailed move history
        
        MatchConfig() = default;
        MatchConfig(int games, bool alt_colors = true)
            : num_games(games), alternate_colors(alt_colors) {}
    };
    
    /**
     * @brief Single game result
     */
    struct GameResult {
        int winner = 0;                      ///< 1=player1, -1=player2, 0=draw
        int final_score_diff = 0;            ///< Final score difference
        int move_count = 0;                  ///< Total number of moves
        double duration_ms = 0.0;            ///< Game duration in milliseconds
        bool player1_was_black = true;      ///< Which player started as black
        
        // Performance metrics
        uint64_t player1_nodes = 0;         ///< Total nodes searched by player1
        uint64_t player2_nodes = 0;         ///< Total nodes searched by player2
        double player1_time_ms = 0.0;       ///< Total search time for player1
        double player2_time_ms = 0.0;       ///< Total search time for player2
        int player1_max_depth = 0;          ///< Maximum depth reached by player1
        int player2_max_depth = 0;          ///< Maximum depth reached by player2
        
        // Move history (optional)
        std::vector<int> moves;             ///< Move sequence (position indices)
        
        /**
         * @brief Calculate average nodes per second for player1
         */
        double player1_nodes_per_sec() const {
            return player1_time_ms > 0 ? (player1_nodes * 1000.0 / player1_time_ms) : 0.0;
        }
        
        /**
         * @brief Calculate average nodes per second for player2
         */
        double player2_nodes_per_sec() const {
            return player2_time_ms > 0 ? (player2_nodes * 1000.0 / player2_time_ms) : 0.0;
        }
    };
    
    /**
     * @brief Complete match result with statistics
     */
    struct MatchResult {
        std::string player1_name;           ///< Name of player1 algorithm
        std::string player2_name;           ///< Name of player2 algorithm
        int player1_wins = 0;               ///< Number of wins for player1
        int player2_wins = 0;               ///< Number of wins for player2
        int draws = 0;                      ///< Number of draws
        double player1_win_rate = 0.0;      ///< Win rate for player1 (0.0-1.0)
        double player2_win_rate = 0.0;      ///< Win rate for player2 (0.0-1.0)
        
        // Game statistics
        double avg_game_length = 0.0;       ///< Average moves per game
        double avg_duration_ms = 0.0;       ///< Average game duration
        std::vector<int> game_lengths;     ///< Individual game lengths
        std::vector<int> score_diffs;       ///< Individual score differences
        std::vector<double> durations;     ///< Individual game durations
        
        // Performance statistics
        double avg_player1_nodes = 0.0;     ///< Average nodes per move (player1)
        double avg_player2_nodes = 0.0;     ///< Average nodes per move (player2)
        double avg_player1_time_ms = 0.0;   ///< Average time per move (player1)
        double avg_player2_time_ms = 0.0;   ///< Average time per move (player2)
        
        // Individual game results
        std::vector<GameResult> games;      ///< Detailed results for each game
        
        /**
         * @brief Calculate total games played
         */
        int total_games() const {
            return player1_wins + player2_wins + draws;
        }
    };
    
    /**
     * @brief Play a complete match between two AI strategies
     * 
     * @param player1 First AI strategy
     * @param player2 Second AI strategy
     * @param config Match configuration
     * @return Complete match result with statistics
     */
    static MatchResult play_match(
        std::shared_ptr<ai::AIStrategy> player1,
        std::shared_ptr<ai::AIStrategy> player2,
        const MatchConfig& config = MatchConfig()
    );
    
    /**
     * @brief Play a single game between two AI strategies
     * 
     * @param player1 First AI strategy
     * @param player2 Second AI strategy
     * @param initial_position Starting board position
     * @param player1_is_black Whether player1 plays as black
     * @param limits Search limits for both players
     * @param config Match configuration
     * @return Game result
     */
    static GameResult play_single_game(
        std::shared_ptr<ai::AIStrategy> player1,
        std::shared_ptr<ai::AIStrategy> player2,
        const core::Board& initial_position,
        bool player1_is_black,
        const ai::SearchLimits& limits,
        const MatchConfig& config = MatchConfig()
    );
    
    /**
     * @brief Play a single game from standard starting position
     */
    static GameResult play_single_game(
        std::shared_ptr<ai::AIStrategy> player1,
        std::shared_ptr<ai::AIStrategy> player2,
        bool player1_is_black,
        const ai::SearchLimits& limits,
        const MatchConfig& config = MatchConfig()
    );
    
private:
    /**
     * @brief Execute one move in a game
     * 
     * @param board Current board state
     * @param strategy AI strategy to use
     * @param limits Search limits
     * @param collect_history Whether to collect move history
     * @return Move position, or -1 for pass
     */
    static int execute_move(
        core::Board& board,
        std::shared_ptr<ai::AIStrategy> strategy,
        const ai::SearchLimits& limits,
        bool collect_history
    );
    
};

} // namespace research
} // namespace reversi

