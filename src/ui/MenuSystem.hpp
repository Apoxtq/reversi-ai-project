/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Menu System (State Pattern)
 * Implements menu state machine with progressive disclosure
 */

#pragma once

#include "UIComponent.hpp"
#include <memory>
#include <vector>

namespace reversi {
namespace ui {

/**
 * @brief Menu state base class (State Pattern)
 * 
 * Each menu screen (MainMenu, Config, etc.) is a state.
 * States handle their own rendering and event handling.
 */
class MenuState {
public:
    virtual ~MenuState() = default;
    
    /**
     * @brief Update state
     * @param dt Delta time
     */
    virtual void update(float dt) = 0;
    
    /**
     * @brief Render state
     * @param target Render target
     */
    virtual void render(sf::RenderTarget& target) = 0;
    
    /**
     * @brief Handle input event
     * @param event SFML event
     * @return true if event was handled
     */
    virtual bool handle_event(const sf::Event& event) = 0;
    
    /**
     * @brief Get next state (if state should transition)
     * @return Next state, or nullptr to stay in current state
     */
    virtual std::unique_ptr<MenuState> get_next_state() = 0;
    
    /**
     * @brief Check if state is finished
     */
    virtual bool is_finished() const = 0;
};

/**
 * @brief Main menu state
 * 
 * Displays main menu with 5 primary buttons:
 * - Play vs AI
 * - Play Local 2P
 * - Online (Week 8)
 * - Benchmark Suite
 * - Exit
 */
class MainMenuState : public MenuState {
public:
    MainMenuState();
    
    void update(float dt) override;
    void render(sf::RenderTarget& target) override;
    bool handle_event(const sf::Event& event) override;
    std::unique_ptr<MenuState> get_next_state() override;
    bool is_finished() const override;
    
    /**
     * @brief Get selected game mode
     */
    enum class Selection {
        NONE,
        VS_AI,
        LOCAL_2P,
        ONLINE,  // Week 8: Network multiplayer
        BENCHMARK,
        EXIT
    };
    
    Selection get_selection() const { return selection_; }
    
private:
    Selection selection_ = Selection::NONE;
    std::vector<std::unique_ptr<Button>> buttons_;
    
    void create_buttons();
};

/**
 * @brief Menu system manager
 * 
 * Manages menu state transitions and rendering.
 * Implements State Pattern for menu navigation.
 */
class MenuSystem {
public:
    MenuSystem();
    
    /**
     * @brief Initialize menu system
     */
    void initialize();
    
    /**
     * @brief Update menu system
     * @param dt Delta time
     */
    void update(float dt);
    
    /**
     * @brief Render menu system
     * @param target Render target
     */
    void render(sf::RenderTarget& target);
    
    /**
     * @brief Handle input event
     * @param event SFML event
     */
    void handle_event(const sf::Event& event);
    
    /**
     * @brief Check if menu is active
     */
    bool is_active() const { return current_state_ != nullptr; }
    
    /**
     * @brief Get current menu selection result
     */
    MainMenuState::Selection get_selection() const;
    
private:
    std::unique_ptr<MenuState> current_state_;
    
    void transition_to_next_state();
};

} // namespace ui
} // namespace reversi

