/*
 * MatchEngine.cpp - AI vs AI Match Engine Implementation
 * COMP390 Honours Year Project - Week 10
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "MatchEngine.hpp"
#include <iostream>
#include <random>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <chrono>

namespace reversi {
namespace research {

MatchEngine::MatchResult MatchEngine::play_match(
    std::shared_ptr<ai::AIStrategy> player1,
    std::shared_ptr<ai::AIStrategy> player2,
    const MatchConfig& config
) {
    MatchResult result;
    result.player1_name = player1->get_name();
    result.player2_name = player2->get_name();
    
    // Initialize random seed if needed
    std::mt19937 rng(config.random_seed == 0 ? 
                     std::chrono::steady_clock::now().time_since_epoch().count() : 
                     config.random_seed);
    
    // Default search limits (can be customized)
    ai::SearchLimits limits;
    limits.max_depth = 6;
    limits.max_time_ms = 5000;
    
    // Play games
    for (int game = 0; game < config.num_games; ++game) {
        // Determine who plays black
        bool player1_is_black = true;
        if (config.alternate_colors) {
            player1_is_black = (game % 2 == 0);
        }
        
        if (config.verbose) {
            std::cout << "Game " << (game + 1) << "/" << config.num_games 
                      << " - " << (player1_is_black ? result.player1_name : result.player2_name)
                      << " as Black... " << std::flush;
        }
        
        // Play single game
        core::Board initial_position; // Standard starting position
        GameResult game_result = play_single_game(
            player1, player2, initial_position, player1_is_black, limits, config
        );
        
        // Update match statistics
        if (game_result.winner == 1) {
            result.player1_wins++;
        } else if (game_result.winner == -1) {
            result.player2_wins++;
        } else {
            result.draws++;
        }
        
        result.game_lengths.push_back(game_result.move_count);
        result.score_diffs.push_back(game_result.final_score_diff);
        result.durations.push_back(game_result.duration_ms);
        result.games.push_back(game_result);
        
        // Accumulate performance metrics
        result.avg_player1_nodes += game_result.player1_nodes;
        result.avg_player2_nodes += game_result.player2_nodes;
        result.avg_player1_time_ms += game_result.player1_time_ms;
        result.avg_player2_time_ms += game_result.player2_time_ms;
        
        if (config.verbose) {
            std::cout << "Winner: " << (game_result.winner == 1 ? result.player1_name :
                                        game_result.winner == -1 ? result.player2_name : "Draw")
                      << " (" << game_result.move_count << " moves, "
                      << std::fixed << std::setprecision(1) << game_result.duration_ms << "ms)"
                      << std::endl;
        }
    }
    
    // Calculate averages
    int total_games = result.total_games();
    if (total_games > 0) {
        result.player1_win_rate = static_cast<double>(result.player1_wins) / total_games;
        result.player2_win_rate = static_cast<double>(result.player2_wins) / total_games;
        
        result.avg_game_length = std::accumulate(
            result.game_lengths.begin(), result.game_lengths.end(), 0.0
        ) / total_games;
        
        result.avg_duration_ms = std::accumulate(
            result.durations.begin(), result.durations.end(), 0.0
        ) / total_games;
        
        result.avg_player1_nodes /= total_games;
        result.avg_player2_nodes /= total_games;
        result.avg_player1_time_ms /= total_games;
        result.avg_player2_time_ms /= total_games;
    }
    
    return result;
}

MatchEngine::GameResult MatchEngine::play_single_game(
    std::shared_ptr<ai::AIStrategy> player1,
    std::shared_ptr<ai::AIStrategy> player2,
    const core::Board& initial_position,
    bool player1_is_black,
    const ai::SearchLimits& limits,
    const MatchConfig& config
) {
    GameResult result;
    result.player1_was_black = player1_is_black;
    
    core::Board board = initial_position;
    int move_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Reset AI states
    player1->reset();
    player2->reset();
    
    // Game loop
    while (!board.is_terminal()) {
        // Get current player's legal moves
        std::vector<int> legal_moves;
        board.get_legal_moves(legal_moves);
        
        if (legal_moves.empty()) {
            board.pass();
            move_count++;
            continue;
        }
        
        // Determine which AI to use
        // In Reversi, black moves first, then white
        // We track: if player1_is_black, then move_count % 2 == 0 means black's turn
        bool is_black_turn = (move_count % 2 == 0);
        bool use_player1 = (player1_is_black == is_black_turn);
        
        std::shared_ptr<ai::AIStrategy> current_strategy = use_player1 ? player1 : player2;
        
        // Execute move
        auto move_start = std::chrono::high_resolution_clock::now();
        int move_pos = execute_move(board, current_strategy, limits, config.collect_move_history);
        auto move_end = std::chrono::high_resolution_clock::now();
        double move_time_ms = std::chrono::duration<double, std::milli>(move_end - move_start).count();
        
        // Collect performance metrics
        const auto& stats = current_strategy->get_stats();
        if (use_player1) {
            result.player1_nodes += stats.nodes_searched;
            result.player1_time_ms += move_time_ms;
            if (stats.depth_reached > result.player1_max_depth) {
                result.player1_max_depth = stats.depth_reached;
            }
        } else {
            result.player2_nodes += stats.nodes_searched;
            result.player2_time_ms += move_time_ms;
            if (stats.depth_reached > result.player2_max_depth) {
                result.player2_max_depth = stats.depth_reached;
            }
        }
        
        // Record move if collecting history
        if (config.collect_move_history && move_pos >= 0) {
            result.moves.push_back(move_pos);
        }
        
        if (move_pos >= 0) {
            board.make_move(move_pos);
            move_count++;
        } else {
            // Pass
            board.pass();
            move_count++;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    result.duration_ms = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    result.move_count = move_count;
    
    // Calculate final score difference
    int player_count = board.count_player();
    int opponent_count = board.count_opponent();
    
    // Determine winner based on final counts and who was black
    // At game end, we know:
    // - player1_is_black tells us who started as black
    // - move_count tells us who made the last move (even = black's turn, odd = white's turn)
    // - player_count/opponent_count are the final piece counts
    
    // Determine which color (black/white) has more pieces
    // If move_count is even, current player is the one who started (black if player1_is_black)
    bool last_move_was_black = (move_count % 2 == 0);
    bool last_player_was_player1 = (player1_is_black == last_move_was_black);
    
    // Calculate score difference from player1's perspective
    if (last_player_was_player1) {
        // Last move was by player1, so player_count is player1's count
        result.final_score_diff = player_count - opponent_count;
        if (player_count > opponent_count) {
            result.winner = 1; // player1 wins
        } else if (player_count < opponent_count) {
            result.winner = -1; // player2 wins
        } else {
            result.winner = 0; // draw
        }
    } else {
        // Last move was by player2, so opponent_count is player1's count
        result.final_score_diff = opponent_count - player_count;
        if (opponent_count > player_count) {
            result.winner = 1; // player1 wins
        } else if (opponent_count < player_count) {
            result.winner = -1; // player2 wins
        } else {
            result.winner = 0; // draw
        }
    }
    
    return result;
}

MatchEngine::GameResult MatchEngine::play_single_game(
    std::shared_ptr<ai::AIStrategy> player1,
    std::shared_ptr<ai::AIStrategy> player2,
    bool player1_is_black,
    const ai::SearchLimits& limits,
    const MatchConfig& config
) {
    core::Board initial_position; // Standard starting position
    return play_single_game(player1, player2, initial_position, player1_is_black, limits, config);
}

int MatchEngine::execute_move(
    core::Board& board,
    std::shared_ptr<ai::AIStrategy> strategy,
    const ai::SearchLimits& limits,
    bool collect_history
) {
    std::vector<int> legal_moves;
    board.get_legal_moves(legal_moves);
    
    if (legal_moves.empty()) {
        return -1; // Pass
    }
    
    // Find best move using AI strategy
    core::Move best_move = strategy->find_best_move(board, limits);
    
    // Validate move
    if (best_move.position < 0 || best_move.position >= 64) {
        // Invalid move, use first legal move as fallback
        return legal_moves[0];
    }
    
    // Check if move is legal
    bool is_legal = false;
    for (int move : legal_moves) {
        if (move == best_move.position) {
            is_legal = true;
            break;
        }
    }
    
    if (!is_legal) {
        // Illegal move, use first legal move as fallback
        return legal_moves[0];
    }
    
    return best_move.position;
}


} // namespace research
} // namespace reversi

