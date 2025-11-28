/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Event Handler (Observer Pattern)
 * Converts SFML input events to game events
 */

#pragma once

#include "../core/Move.hpp"
#include <SFML/Window/Event.hpp>
#include <functional>
#include <vector>
#include <memory>

namespace reversi {
namespace ui {

/**
 * @brief Game event types
 */
enum class GameEventType {
    MOVE_SELECTED,      // Player selected a move
    MOVE_INVALID,       // Invalid move attempted
    GAME_OVER,          // Game finished
    PAUSE_REQUESTED,    // Pause requested
    UNDO_REQUESTED,     // Undo requested
    MENU_REQUESTED,     // Return to menu
    SETTINGS_CHANGED    // Settings changed
};

/**
 * @brief Game event structure
 */
struct GameEvent {
    GameEventType type;
    core::Move move;           // For MOVE_SELECTED
    std::string message;        // For error messages
    int position = -1;          // Board position (0-63)
    
    GameEvent(GameEventType t) : type(t) {}
    GameEvent(GameEventType t, const core::Move& m) : type(t), move(m) {}
    GameEvent(GameEventType t, int pos) : type(t), position(pos) {}
};

/**
 * @brief Event handler class (Observer Pattern)
 * 
 * Converts SFML input events to game events.
 * Does NOT modify game state directly - only emits events.
 * 
 * Responsibilities:
 * - Mouse click detection
 * - Keyboard input handling
 * - Coordinate conversion
 * - Event emission
 * 
 * Does NOT:
 * - Modify game state (handled by GameState)
 * - Render anything (handled by View)
 */
class EventHandler {
public:
    /**
     * @brief Construct event handler
     */
    EventHandler();
    
    /**
     * @brief Process SFML event
     * @param event SFML event
     * @param board_renderer Board renderer for coordinate conversion
     * @return Game event, or nullptr if not handled
     */
    std::unique_ptr<GameEvent> process_event(const sf::Event& event, 
                                             class BoardRenderer* board_renderer);
    
    /**
     * @brief Set callback for game events
     */
    void set_event_callback(std::function<void(const GameEvent&)> callback) {
        event_callback_ = callback;
    }
    
    /**
     * @brief Enable/disable input
     */
    void set_enabled(bool enabled) { enabled_ = enabled; }
    bool is_enabled() const { return enabled_; }
    
private:
    bool enabled_ = true;
    std::function<void(const GameEvent&)> event_callback_;
    
    // Mouse state
    sf::Vector2i last_mouse_pos_;
    bool mouse_pressed_ = false;
    
    // Helper methods
    std::unique_ptr<GameEvent> handle_mouse_click(const sf::Event::MouseButtonEvent& event,
                                                   class BoardRenderer* board_renderer);
    std::unique_ptr<GameEvent> handle_keyboard(const sf::Event::KeyEvent& event);
};

} // namespace ui
} // namespace reversi

