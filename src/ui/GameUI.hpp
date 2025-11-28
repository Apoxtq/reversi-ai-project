/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Game UI Controller (MVC Controller)
 * Main game loop and component coordination
 */

#pragma once

#include "../core/Board.hpp"
#include "../ai/AIStrategy.hpp"
#include "GameState.hpp"
#include "BoardRenderer.hpp"
#include "MenuSystem.hpp"
#include "EventHandler.hpp"
#include "UIComponent.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <future>
#include <optional>

// Forward declarations
namespace reversi {
namespace ui {
    class AnimationSystem;
}
}

namespace reversi {
namespace ui {

/**
 * @brief Game UI Controller (MVC Controller)
 * 
 * Main controller coordinating all UI components.
 * Implements MVC architecture:
 * - Controller: GameUI (this class)
 * - View: BoardRenderer, MenuSystem
 * - Model: Board, GameState
 * 
 * Responsibilities:
 * - Window management
 * - Event loop (60 FPS)
 * - Component coordination
 * - AI integration (async)
 * - State transitions
 */
class GameUI {
public:
    /**
     * @brief Construct GameUI
     * @param window SFML render window
     */
    explicit GameUI(sf::RenderWindow& window);
    
    /**
     * @brief Initialize UI system
     */
    void initialize();
    
    /**
     * @brief Run main game loop
     */
    void run();
    
    /**
     * @brief Start new game
     * @param mode Game mode
     * @param ai_strategy AI strategy (for vs AI mode)
     */
    void start_game(GameMode mode, std::unique_ptr<ai::AIStrategy> ai_strategy = nullptr);
    
    /**
     * @brief Return to main menu
     */
    void return_to_menu();
    
private:
    // Window and timing
    sf::RenderWindow& window_;
    sf::Clock frame_clock_;
    float frame_time_ = 0.0f;
    
    // Core components
    std::unique_ptr<GameState> game_state_;
    std::unique_ptr<BoardRenderer> board_renderer_;
    std::unique_ptr<MenuSystem> menu_system_;
    std::unique_ptr<EventHandler> event_handler_;
    
    // AI integration
    std::unique_ptr<ai::AIStrategy> ai_strategy_;
    std::future<core::Move> ai_future_;
    bool ai_thinking_ = false;
    sf::Clock ai_think_clock_;
    
    // UI state
    enum class UIState {
        MENU,
        GAME,
        GAME_OVER
    };
    UIState current_state_ = UIState::MENU;
    
    // UI panels
    std::unique_ptr<Panel> status_panel_;
    std::unique_ptr<Panel> game_over_panel_;
    std::unique_ptr<Button> home_button_;
    std::unique_ptr<Button> undo_button_;
    std::unique_ptr<Button> play_again_button_;
    
    // Animation system
    std::unique_ptr<class AnimationSystem> animation_system_;
    
    // Game loop methods
    void handle_events();
    void update(float dt);
    void render();
    
    // Game state methods
    void process_game_event(const GameEvent& event);
    void make_move(const core::Move& move);
    void update_ai();
    void check_game_over();
    
    // UI update methods
    void update_status_panel();
    void create_status_panel();
    void create_control_buttons();
    
    // Helper methods
    void transition_to_game();
    void transition_to_menu();
    void transition_to_game_over();
};

} // namespace ui
} // namespace reversi

