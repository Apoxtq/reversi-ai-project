/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Game State Management
 * Separates game flow state from Board data model (MVC architecture)
 */

#pragma once

#include "../core/Board.hpp"
#include "../core/Move.hpp"
#include <vector>
#include <string>
#include <optional>

namespace reversi {
namespace ui {

/**
 * @brief Game mode enumeration
 */
enum class GameMode {
    LOCAL_2P,      // Local two-player
    VS_AI,         // Human vs AI
    AI_VS_AI,      // AI vs AI
    ONLINE         // Network multiplayer (Week 8)
};

/**
 * @brief Game phase enumeration
 */
enum class GamePhase {
    MENU,          // Main menu
    CONFIG,        // Game configuration
    PLAYING,       // Active game
    GAME_OVER,     // Game finished
    PAUSED         // Game paused
};

/**
 * @brief Player color (for UI display)
 */
enum class PlayerColor {
    BLACK,
    WHITE
};

/**
 * @brief Game state management class
 * 
 * Manages game flow state separate from Board data model.
 * Follows MVC architecture: Board is Model (data), GameState is Controller state.
 * 
 * Responsibilities:
 * - Current player tracking
 * - Game mode and phase
 * - Move history
 * - Game statistics
 * 
 * Does NOT manage:
 * - Board data (handled by Board class)
 * - Rendering (handled by BoardRenderer)
 */
class GameState {
public:
    /**
     * @brief Construct game state
     * @param mode Game mode
     * @param initial_board Initial board position
     */
    GameState(GameMode mode, const core::Board& initial_board);
    
    /**
     * @brief Get current player color
     */
    PlayerColor get_current_player() const { return current_player_; }
    
    /**
     * @brief Get game mode
     */
    GameMode get_mode() const { return mode_; }
    
    /**
     * @brief Get game phase
     */
    GamePhase get_phase() const { return phase_; }
    
    /**
     * @brief Get move history
     */
    const std::vector<core::Move>& get_move_history() const { return move_history_; }
    
    /**
     * @brief Get move count
     */
    size_t get_move_count() const { return move_history_.size(); }
    
    /**
     * @brief Check if it's a specific player's turn
     */
    bool is_player_turn(PlayerColor player) const {
        return current_player_ == player;
    }
    
    /**
     * @brief Make a move (validates and updates state)
     * @param move Move to make
     * @param board Board to apply move to (will be modified)
     * @return true if move was valid and applied
     */
    bool make_move(const core::Move& move, core::Board& board);
    
    /**
     * @brief Switch to next player
     */
    void switch_player();
    
    /**
     * @brief Check if game is over
     * @param board Current board state
     * @return true if game is finished
     */
    bool check_game_over(const core::Board& board);
    
    /**
     * @brief Get winner (if game is over)
     * @param board Current board state
     * @return Winner color, or nullopt if draw/not finished
     */
    std::optional<PlayerColor> get_winner(const core::Board& board) const;
    
    /**
     * @brief Undo last move (if allowed)
     * @param board Board to undo move on
     * @return true if undo was successful
     */
    bool undo_move(core::Board& board);
    
    /**
     * @brief Check if undo is allowed
     */
    bool can_undo() const;
    
    /**
     * @brief Set game phase
     */
    void set_phase(GamePhase phase) { phase_ = phase; }
    
    /**
     * @brief Reset game state
     * @param board Board to reset
     */
    void reset(const core::Board& initial_board);
    
    /**
     * @brief Get game statistics
     */
    struct Statistics {
        size_t total_moves = 0;
        size_t black_moves = 0;
        size_t white_moves = 0;
        size_t passes = 0;
    };
    
    Statistics get_statistics() const;
    
private:
    GameMode mode_;                    // Game mode
    GamePhase phase_;                   // Current game phase
    PlayerColor current_player_;        // Current player (Black/White)
    
    std::vector<core::Move> move_history_;  // Move history
    std::vector<core::Board> board_history_; // Board history for undo
    
    // Helper: Convert Board's player perspective to UI player color
    PlayerColor board_to_ui_color(const core::Board& board) const;
};

} // namespace ui
} // namespace reversi

