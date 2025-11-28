/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Game State Management Implementation
 */

#include "GameState.hpp"
#include <algorithm>
#include <optional>

namespace reversi {
namespace ui {

GameState::GameState(GameMode mode, const core::Board& initial_board)
    : mode_(mode)
    , phase_(GamePhase::PLAYING)
    , current_player_(PlayerColor::BLACK)
{
    board_history_.push_back(initial_board);
}

bool GameState::make_move(const core::Move& move, core::Board& board) {
    if (phase_ != GamePhase::PLAYING) {
        return false;
    }
    
    // Validate move
    auto legal_moves = board.get_legal_moves();
    bool is_legal = std::any_of(legal_moves.begin(), legal_moves.end(),
        [&move](const core::Move& m) { return m.position == move.position; });
    
    if (!is_legal && !move.is_pass()) {
        return false;
    }
    
    // Apply move
    if (move.is_pass()) {
        board.pass();
    } else {
        board.make_move(move.position);
    }
    
    // Update state
    move_history_.push_back(move);
    board_history_.push_back(board);
    switch_player();
    
    // Check for game over
    if (check_game_over(board)) {
        phase_ = GamePhase::GAME_OVER;
    }
    
    return true;
}

void GameState::switch_player() {
    current_player_ = (current_player_ == PlayerColor::BLACK) 
        ? PlayerColor::WHITE 
        : PlayerColor::BLACK;
}

bool GameState::check_game_over(const core::Board& board) {
    if (board.is_terminal()) {
        return true;
    }
    
    // Check if both players have no moves (consecutive passes)
    auto moves = board.get_legal_moves();
    if (moves.empty()) {
        // Current player must pass
        core::Board test_board = board;
        test_board.pass();
        auto opponent_moves = test_board.get_legal_moves();
        return opponent_moves.empty(); // Both players have no moves
    }
    
    return false;
}

std::optional<PlayerColor> GameState::get_winner(const core::Board& board) const {
    if (phase_ != GamePhase::GAME_OVER) {
        return std::nullopt;
    }
    
    int black_count = board.count_player();
    int white_count = board.count_opponent();
    
    // Note: Board's player/opponent perspective may differ from UI perspective
    // This is a simplified version - may need adjustment based on actual game state
    
    if (black_count > white_count) {
        return PlayerColor::BLACK;
    } else if (white_count > black_count) {
        return PlayerColor::WHITE;
    } else {
        return std::nullopt; // Draw
    }
}

bool GameState::undo_move(core::Board& board) {
    // Undo not allowed in network mode
    if (mode_ == GameMode::ONLINE) {
        return false;
    }
    
    if (board_history_.size() < 2) {
        return false; // Nothing to undo
    }
    
    // Remove last move
    move_history_.pop_back();
    board_history_.pop_back();
    
    // Restore previous board state
    board = board_history_.back();
    
    // Switch player back
    switch_player();
    
    // Update phase if needed
    if (phase_ == GamePhase::GAME_OVER) {
        phase_ = GamePhase::PLAYING;
    }
    
    return true;
}

bool GameState::can_undo() const {
    return mode_ != GameMode::ONLINE && board_history_.size() > 1;
}

void GameState::reset(const core::Board& initial_board) {
    move_history_.clear();
    board_history_.clear();
    board_history_.push_back(initial_board);
    current_player_ = PlayerColor::BLACK;
    phase_ = GamePhase::PLAYING;
}

GameState::Statistics GameState::get_statistics() const {
    Statistics stats;
    stats.total_moves = move_history_.size();
    
    PlayerColor player = PlayerColor::BLACK;
    for (const auto& move : move_history_) {
        if (move.is_pass()) {
            stats.passes++;
        } else if (player == PlayerColor::BLACK) {
            stats.black_moves++;
        } else {
            stats.white_moves++;
        }
        player = (player == PlayerColor::BLACK) ? PlayerColor::WHITE : PlayerColor::BLACK;
    }
    
    return stats;
}

PlayerColor GameState::board_to_ui_color(const core::Board& board) const {
    // This is a placeholder - actual implementation depends on how Board's
    // player/opponent perspective maps to UI Black/White
    // For now, assume Board's player is Black in UI
    return PlayerColor::BLACK;
}

} // namespace ui
} // namespace reversi

