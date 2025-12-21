/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Game UI Controller Implementation
 */

#include "GameUI.hpp"
#include "UIStyle.hpp"
#include "AnimationSystem.hpp"
#include "../core/Board.hpp"
#include "../ai/MinimaxEngine.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <sstream>

namespace reversi {
namespace ui {

GameUI::GameUI(sf::RenderWindow& window)
    : window_(window)
{
}

void GameUI::initialize() {
    std::cout << "    [GameUI::initialize] Step 1: Creating MenuSystem..." << std::endl;
    try {
    // Initialize menu system
    menu_system_ = std::make_unique<MenuSystem>();
        std::cout << "    [GameUI::initialize] Step 2: MenuSystem created, calling initialize()..." << std::endl;
    menu_system_->initialize();
        std::cout << "    [GameUI::initialize] Step 3: MenuSystem initialized successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to initialize menu system: " << e.what() << std::endl;
        std::cerr << "Exception type: " << typeid(e).name() << std::endl;
        // Continue - menu may still work partially
    } catch (...) {
        std::cerr << "Warning: Unknown exception during menu system initialization!" << std::endl;
    }
    
    try {
    // Initialize event handler
    event_handler_ = std::make_unique<EventHandler>();
    event_handler_->set_event_callback([this](const GameEvent& event) {
        process_game_event(event);
    });
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to initialize event handler: " << e.what() << std::endl;
        // Continue - basic events may still work
    }
    
    try {
    // Initialize animation system
    animation_system_ = std::make_unique<AnimationSystem>();
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to initialize animation system: " << e.what() << std::endl;
        // Continue - animations will be disabled
    }
    
    try {
    // Initialize board renderer
    float board_x = UIStyle::SPACING_LG;
    float board_y = UIStyle::CONTROL_BAR_HEIGHT + UIStyle::SPACING_LG;
    board_renderer_ = std::make_unique<BoardRenderer>(
        sf::Vector2f(board_x, board_y),
        UIStyle::BOARD_SIZE
    );
        if (animation_system_) {
    board_renderer_->set_animation_system(animation_system_.get());
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to initialize board renderer: " << e.what() << std::endl;
        // Continue - game may still work without board rendering
    }
}

void GameUI::run() {
    while (window_.isOpen()) {
        handle_events();
        
        float dt = frame_clock_.restart().asSeconds();
        frame_time_ = dt;
        update(dt);
        
        render();
    }
}

void GameUI::handle_events() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
            return;
        }
        
        if (current_state_ == UIState::MENU) {
            if (!menu_system_) {
                // Menu system not initialized - skip event handling
                continue;
            }
            
            menu_system_->handle_event(event);
            
            // Check for menu selection
            auto selection = menu_system_->get_selection();
            if (selection == MainMenuState::Selection::VS_AI) {
                // Create AI strategy with default difficulty (Medium)
                auto ai_strategy = std::make_unique<ai::MinimaxEngine>(
                    ai::MinimaxEngine::Config(5, true, true, 20) // Depth 5, Alpha-Beta, TT
                );
                start_game(GameMode::VS_AI, std::move(ai_strategy));
            } else if (selection == MainMenuState::Selection::LOCAL_2P) {
                start_game(GameMode::LOCAL_2P);
            } else if (selection == MainMenuState::Selection::ONLINE) {
                // Network game - start as host (Week 8)
                // TODO: Integrate with NetworkLobbyState for full UI (create/join room)
                start_network_game(true);  // Start as host for now
            } else if (selection == MainMenuState::Selection::EXIT) {
                window_.close();
            }
        } else if (current_state_ == UIState::GAME || current_state_ == UIState::GAME_OVER) {
            // Handle game events
            if (!event_handler_) {
                // Event handler not initialized - skip event processing
                continue;
            }
            
            auto game_event = event_handler_->process_event(event, board_renderer_.get());
            if (game_event) {
                process_game_event(*game_event);
            }
            
            // Handle UI button events
            if (home_button_) {
                home_button_->handle_event(event);
                if (home_button_->was_clicked()) {
                    home_button_->reset_click();
                    return_to_menu();
                }
            }
            
            if (undo_button_ && undo_button_->is_enabled()) {
                undo_button_->handle_event(event);
                if (undo_button_->was_clicked()) {
                    undo_button_->reset_click();
                    if (game_state_ && game_state_->can_undo()) {
                        // Reconstruct board to undo point
                        core::Board board;
                        auto history = game_state_->get_move_history();
                        if (history.size() > 1) {
                            // Reconstruct board up to second-to-last move
                            for (size_t i = 0; i < history.size() - 1; ++i) {
                                if (history[i].is_pass()) {
                                    board.pass();
                                } else {
                                    board.make_move(history[i].position);
                                }
                            }
                        }
                        game_state_->undo_move(board);
                        board_renderer_->set_board(board);
                        
                        // Update legal moves
                        std::vector<int> legal_positions;
                        board.get_legal_moves(legal_positions);
                        std::vector<core::Move> legal_moves;
                        for (int pos : legal_positions) {
                            legal_moves.emplace_back(pos);
                        }
                        board_renderer_->set_legal_moves(legal_moves);
                    }
                }
            }
            
            // Handle game over screen buttons
            if (current_state_ == UIState::GAME_OVER) {
                if (play_again_button_) {
                    play_again_button_->handle_event(event);
                    if (play_again_button_->was_clicked()) {
                        play_again_button_->reset_click();
                        if (game_state_) {
                            GameMode mode = game_state_->get_mode();
                            start_game(mode, std::move(ai_strategy_));
                        }
                    }
                }
            }
        }
    }
}

void GameUI::update(float dt) {
    if (current_state_ == UIState::MENU) {
        if (menu_system_) {
        menu_system_->update(dt);
        }
    } else if (current_state_ == UIState::GAME) {
        // Update animation system
        if (animation_system_) {
            animation_system_->update(dt);
        }
        
        // Update board renderer
        if (board_renderer_) {
            board_renderer_->update(dt);
        }
        
        // Update network game (Week 8)
        if (is_network_mode_ && network_game_) {
            network_game_->update(dt);
            update_network_status();
        }
        
        // Update network error message display timer
        if (network_error_display_time_ > 0.0f) {
            network_error_display_time_ -= dt;
            if (network_error_display_time_ <= 0.0f) {
                network_error_message_.clear();
            }
        }
        
        // Update AI (only if not network mode)
        if (!is_network_mode_) {
            update_ai();
        }
        
        // Update status panel
        update_status_panel();
        
        // Update UI components
        if (status_panel_) {
            status_panel_->update(dt);
        }
        if (home_button_) {
            home_button_->update(dt);
        }
        if (undo_button_) {
            undo_button_->update(dt);
        }
        
        // Check game over
        check_game_over();
    } else if (current_state_ == UIState::GAME_OVER) {
        // Update game over panel
        if (game_over_panel_) {
            game_over_panel_->update(dt);
        }
        if (play_again_button_) {
            play_again_button_->update(dt);
        }
        if (home_button_) {
            home_button_->update(dt);
        }
    }
}

void GameUI::render() {
    window_.clear(UIStyle::BACKGROUND_L1);
    
    if (current_state_ == UIState::MENU) {
        if (menu_system_) {
        menu_system_->render(window_);
        }
    } else if (current_state_ == UIState::GAME || current_state_ == UIState::GAME_OVER) {
        // Draw control bar
        sf::RectangleShape control_bar(sf::Vector2f(UIStyle::WINDOW_WIDTH, UIStyle::CONTROL_BAR_HEIGHT));
        control_bar.setFillColor(UIStyle::BACKGROUND_L2);
        control_bar.setPosition(0, 0);
        window_.draw(control_bar);
        
        // Draw board
        if (board_renderer_) {
            window_.draw(*board_renderer_);
        }
        
        // Draw status panel
        if (status_panel_) {
            window_.draw(*status_panel_);
        }
        
        // Draw network status (Week 8)
        if (is_network_mode_) {
            render_network_status(window_);
        }
        
        // Draw network error message (Week 8)
        if (!network_error_message_.empty() && network_error_display_time_ > 0.0f) {
            render_network_error_message(window_);
        }
        
        // Draw control buttons
        if (home_button_) {
            window_.draw(*home_button_);
        }
        if (undo_button_) {
            window_.draw(*undo_button_);
        }
        
        // Draw game over panel if game is over
        if (current_state_ == UIState::GAME_OVER && game_over_panel_) {
            window_.draw(*game_over_panel_);
        }
        if (play_again_button_) {
            window_.draw(*play_again_button_);
        }
    }
    
    window_.display();
}

void GameUI::start_game(GameMode mode, std::unique_ptr<ai::AIStrategy> ai_strategy) {
    // Initialize game state
    core::Board initial_board;
    game_state_ = std::make_unique<GameState>(mode, initial_board);
    
    // Set AI strategy
    ai_strategy_ = std::move(ai_strategy);
    ai_thinking_ = false;
    
    // Update board renderer
    if (board_renderer_) {
        board_renderer_->set_board(initial_board);
        std::vector<int> legal_positions;
        initial_board.get_legal_moves(legal_positions);
        std::vector<core::Move> legal_moves;
        for (int pos : legal_positions) {
            legal_moves.emplace_back(pos);
        }
        board_renderer_->set_legal_moves(legal_moves);
    }
    
    // Create UI panels
    create_status_panel();
    create_control_buttons();
    
    // Transition to game state
    transition_to_game();
}

void GameUI::start_network_game(bool is_host, const std::string& room_code,
                                const std::string& host_ip, unsigned short host_port) {
    // Create network game
    network::NetworkRole role = is_host ? network::NetworkRole::HOST : network::NetworkRole::CLIENT;
    network_game_ = std::make_unique<network::NetworkGame>(role);
    
    // Set up callbacks
    network_game_->set_move_received_callback([this](const core::Move& move, uint64_t board_hash) {
        handle_network_move(move, board_hash);
    });
    
    network_game_->set_state_changed_callback([this](network::NetworkGameState state) {
        handle_network_state_change(state);
    });
    
    network_game_->set_error_callback([this](const std::string& error) {
        handle_network_error(error);
    });
    
    // Start network connection
    if (is_host) {
        std::string code = network_game_->start_as_host(0);
        if (code.empty()) {
            std::cerr << "Failed to create room\n";
            return;
        }
        std::cout << "Room created: " << code << "\n";
    } else {
        bool connected = network_game_->start_as_client(room_code, host_ip, host_port);
        if (!connected) {
            std::cerr << "Failed to connect to host\n";
            return;
        }
    }
    
    // Start game in online mode
    start_game(GameMode::ONLINE);
}

void GameUI::return_to_menu() {
    transition_to_menu();
}

void GameUI::process_game_event(const GameEvent& event) {
    switch (event.type) {
        case GameEventType::MOVE_SELECTED:
            make_move(event.move);
            break;
            
        case GameEventType::UNDO_REQUESTED:
            if (game_state_ && game_state_->can_undo() && board_renderer_) {
                // Reconstruct board to undo point
                core::Board board;
                auto history = game_state_->get_move_history();
                if (history.size() > 1) {
                    for (size_t i = 0; i < history.size() - 1; ++i) {
                        if (history[i].is_pass()) {
                            board.pass();
                        } else {
                            board.make_move(history[i].position);
                        }
                    }
                }
                game_state_->undo_move(board);
                board_renderer_->set_board(board);
                
                // Update legal moves
                std::vector<int> legal_positions;
                board.get_legal_moves(legal_positions);
                std::vector<core::Move> legal_moves;
                for (int pos : legal_positions) {
                    legal_moves.emplace_back(pos);
                }
                board_renderer_->set_legal_moves(legal_moves);
            }
            break;
            
        case GameEventType::MENU_REQUESTED:
            return_to_menu();
            break;
            
        default:
            break;
    }
}

void GameUI::make_move(const core::Move& move) {
    if (!game_state_ || !board_renderer_) return;
    
    // If network mode, send move over network
    if (is_network_mode_ && network_game_ && network_game_->is_connected()) {
        // Reconstruct current board state
        core::Board board;
        for (const auto& past_move : game_state_->get_move_history()) {
            if (past_move.is_pass()) {
                board.pass();
            } else {
                board.make_move(past_move.position);
            }
        }
        
        // Send move over network
        if (!network_game_->send_move(move, board)) {
            std::cerr << "Failed to send move over network\n";
            return;
        }
    }
    
    // Get current board state from renderer
    // Note: In a more complete implementation, we'd maintain board state separately
    // For now, we'll create a new board and apply all moves from history
    core::Board board;
    for (const auto& past_move : game_state_->get_move_history()) {
        if (past_move.is_pass()) {
            board.pass();
        } else {
            board.make_move(past_move.position);
        }
    }
    
    // Validate move
    std::vector<int> legal_positions;
    board.get_legal_moves(legal_positions);
    bool is_legal = std::any_of(legal_positions.begin(), legal_positions.end(),
        [&move](int pos) { return pos == move.position; });
    
    if (!is_legal && !move.is_pass()) {
        // Invalid move - show error
        return;
    }
    
    // Make move
    if (game_state_->make_move(move, board)) {
        board_renderer_->set_board(board);
        board_renderer_->set_last_move(move.position);
        
        // Update legal moves
        std::vector<int> new_legal_positions;
        board.get_legal_moves(new_legal_positions);
        std::vector<core::Move> new_legal_moves;
        for (int pos : new_legal_positions) {
            new_legal_moves.emplace_back(pos);
        }
        board_renderer_->set_legal_moves(new_legal_moves);
        
        // Check if AI should move
        if (game_state_->get_mode() == GameMode::VS_AI && 
            game_state_->get_current_player() == PlayerColor::WHITE) {
            // AI's turn
            update_ai();
        }
    }
}

void GameUI::update_ai() {
    if (!ai_strategy_ || !game_state_) return;
    
    // Check if AI should move
    if (game_state_->get_mode() == GameMode::VS_AI && 
        game_state_->get_current_player() == PlayerColor::WHITE) {
        
        if (!ai_thinking_) {
            // Start AI search
            ai_thinking_ = true;
            ai_think_clock_.restart();
            
            // Reconstruct current board state
            core::Board board;
            for (const auto& past_move : game_state_->get_move_history()) {
                if (past_move.is_pass()) {
                    board.pass();
                } else {
                    board.make_move(past_move.position);
                }
            }
            
            ai::SearchLimits limits;
            limits.max_depth = 6;
            limits.max_time_ms = 3000;
            
            // Start async AI search
            ai_future_ = std::async(std::launch::async, [this, board, limits]() {
                return ai_strategy_->find_best_move(board, limits);
            });
        }
    }
    
    // Check if AI search is complete
    if (ai_thinking_ && ai_future_.valid()) {
        auto status = ai_future_.wait_for(std::chrono::milliseconds(0));
        if (status == std::future_status::ready) {
            core::Move ai_move = ai_future_.get();
            ai_thinking_ = false;
            make_move(ai_move);
        }
    }
}

void GameUI::check_game_over() {
    if (!game_state_ || !board_renderer_) return;
    
    // Reconstruct current board state
    core::Board board;
    for (const auto& past_move : game_state_->get_move_history()) {
        if (past_move.is_pass()) {
            board.pass();
        } else {
            board.make_move(past_move.position);
        }
    }
    
    if (game_state_->check_game_over(board)) {
        transition_to_game_over();
    }
}

void GameUI::update_status_panel() {
    if (!status_panel_ || !game_state_ || !board_renderer_) return;
    
    // Update status panel with current game state
    // This is a simplified version - in a full implementation,
    // we would update text labels showing:
    // - Current player (Black/White)
    // - Score (Black: X, White: Y)
    // - AI thinking status (if vs AI mode)
    // - Game mode
    // - Move count
    
    // For now, panel is created but content updates would require
    // additional UI components (Text labels, etc.)
}

void GameUI::create_status_panel() {
    float panel_x = UIStyle::BOARD_SIZE + UIStyle::SPACING_LG * 2;
    float panel_y = UIStyle::CONTROL_BAR_HEIGHT + UIStyle::SPACING_LG;
    
    status_panel_ = std::make_unique<Panel>(
        sf::Vector2f(panel_x, panel_y),
        sf::Vector2f(UIStyle::PANEL_WIDTH, 
                     UIStyle::WINDOW_HEIGHT - panel_y - UIStyle::FOOTER_HEIGHT)
    );
}

void GameUI::create_control_buttons() {
    // Home button
    home_button_ = std::make_unique<Button>(
        "Home",
        sf::Vector2f(UIStyle::SPACING_MD, UIStyle::SPACING_SM),
        sf::Vector2f(100.0f, 30.0f),
        Button::Style::SECONDARY
    );
    
    // Undo button
    undo_button_ = std::make_unique<Button>(
        "Undo",
        sf::Vector2f(120.0f, UIStyle::SPACING_SM),
        sf::Vector2f(100.0f, 30.0f),
        Button::Style::SECONDARY
    );
    
    if (game_state_ && !game_state_->can_undo()) {
        undo_button_->set_enabled(false);
    }
}

void GameUI::transition_to_game() {
    current_state_ = UIState::GAME;
    if (event_handler_) {
    event_handler_->set_enabled(true);
    }
}

void GameUI::transition_to_menu() {
    current_state_ = UIState::MENU;
    game_state_.reset();
    board_renderer_.reset();
    status_panel_.reset();
    game_over_panel_.reset();
    home_button_.reset();
    undo_button_.reset();
    play_again_button_.reset();
    ai_strategy_.reset();
    ai_thinking_ = false;
    if (animation_system_) {
        animation_system_->clear();
    }
}

void GameUI::transition_to_game_over() {
    current_state_ = UIState::GAME_OVER;
    event_handler_->set_enabled(false);
    
    // Create game over panel
    float panel_x = (UIStyle::WINDOW_WIDTH - 400.0f) / 2.0f;
    float panel_y = (UIStyle::WINDOW_HEIGHT - 300.0f) / 2.0f;
    
    game_over_panel_ = std::make_unique<Panel>(
        sf::Vector2f(panel_x, panel_y),
        sf::Vector2f(400.0f, 300.0f)
    );
    
    // Create play again button
    play_again_button_ = std::make_unique<Button>(
        "Play Again",
        sf::Vector2f(panel_x + 50.0f, panel_y + 200.0f),
        sf::Vector2f(120.0f, 40.0f),
        Button::Style::PRIMARY
    );
    play_again_button_->set_callback([this]() {
        // Restart game with same mode
        if (game_state_) {
            GameMode mode = game_state_->get_mode();
            start_game(mode, std::move(ai_strategy_));
        }
    });
    
    // Home button already exists, just enable it
    if (home_button_) {
        home_button_->set_enabled(true);
    }
}

// Network methods (Week 8)
void GameUI::handle_network_move(const core::Move& move, uint64_t board_hash) {
    if (!game_state_ || !board_renderer_) return;
    
    // Reconstruct current board state
    core::Board board;
    for (const auto& past_move : game_state_->get_move_history()) {
        if (past_move.is_pass()) {
            board.pass();
        } else {
            board.make_move(past_move.position);
        }
    }
    
    // Verify board hash
    uint64_t local_hash = board.hash();
    if (local_hash != board_hash) {
        // State mismatch - request sync
        if (network_game_) {
            network_game_->request_sync(board_hash);
        }
        std::cerr << "Board state mismatch detected! Local: " << local_hash 
                  << ", Remote: " << board_hash << "\n";
        return;
    }
    
    // Apply move (this will update game state and board renderer)
    // Note: We need to apply the move without sending it back over network
    bool was_network_mode = is_network_mode_;
    is_network_mode_ = false;  // Temporarily disable to avoid sending back
    make_move(move);
    is_network_mode_ = was_network_mode;
}

void GameUI::handle_network_state_change(network::NetworkGameState state) {
    switch (state) {
        case network::NetworkGameState::CONNECTED:
            std::cout << "Network: Connected\n";
            break;
        case network::NetworkGameState::PLAYING:
            std::cout << "Network: Game started\n";
            break;
        case network::NetworkGameState::DISCONNECTED:
            std::cout << "Network: Disconnected\n";
            break;
        case network::NetworkGameState::ERROR_STATE:  // Error state
            std::cerr << "Network: Error state\n";
            break;
        default:
            break;
    }
}

void GameUI::handle_network_error(const std::string& error) {
    std::cerr << "Network error: " << error << "\n";
    
    // Store error message for UI display
    network_error_message_ = error;
    network_error_display_time_ = NETWORK_ERROR_DISPLAY_DURATION;
    
    // Also log to console for debugging
    std::cout << "[Network Error] " << error << "\n";
}

void GameUI::update_network_status() {
    // Network status is updated in render_network_status()
    // This method can be used for additional status updates if needed
}

void GameUI::render_network_status(sf::RenderTarget& target) {
    if (!network_game_) return;
    
    // Get network status
    uint32_t rtt = network_game_->get_rtt_ms();
    bool connected = network_game_->is_connected();
    network::NetworkGameState state = network_game_->get_state();
    
    // Don't use static font - avoid initialization order issues
    // We'll render without text for now (just use colored indicators)
    
    // Position: top-right corner
    float x = UIStyle::WINDOW_WIDTH - 250.0f;
    float y = UIStyle::SPACING_MD;
    
    // Status indicator circle (🟢🟡🔴)
    float circle_radius = 8.0f;
    sf::CircleShape status_indicator(circle_radius);
    status_indicator.setPosition(x, y + 5.0f);
    
    // Color based on connection status and RTT
    // Note: ERROR is a Windows macro, so we use enum value directly
    using State = network::NetworkGameState;
    bool is_error = (state == State::DISCONNECTED || 
                     state == static_cast<State>(6));  // ERROR state (6th enum value)
    if (!connected || is_error) {
        status_indicator.setFillColor(sf::Color::Red);  // 🔴 Disconnected/Error
    } else if (rtt == 0 || rtt < 50) {
        status_indicator.setFillColor(sf::Color::Green);  // 🟢 LAN (excellent)
    } else if (rtt < 150) {
        status_indicator.setFillColor(sf::Color::Yellow);  // 🟡 Internet (good)
    } else {
        status_indicator.setFillColor(sf::Color(255, 165, 0));  // 🟠 High latency
    }
    
    target.draw(status_indicator);
    
    // Status text (if font is available, otherwise skip text)
    // Note: In a full implementation, we would use UIStyle::get_font()
    // For now, we'll render the indicator circle which works without font
    // Text rendering can be added when font system is fully integrated
    
    // Alternative: Render simple status using shapes if font not available
    // For now, the colored circle indicator is sufficient
}

void GameUI::render_network_error_message(sf::RenderTarget& target) {
    if (network_error_message_.empty()) return;
    
    // Render error message as a semi-transparent panel at the top center
    float panel_width = 600.0f;
    float panel_height = 60.0f;
    float panel_x = (UIStyle::WINDOW_WIDTH - panel_width) / 2.0f;
    float panel_y = UIStyle::SPACING_MD;
    
    // Background panel (semi-transparent red)
    sf::RectangleShape error_panel(sf::Vector2f(panel_width, panel_height));
    error_panel.setPosition(panel_x, panel_y);
    error_panel.setFillColor(sf::Color(217, 83, 79, 200));  // ERROR_RED with alpha
    error_panel.setOutlineColor(sf::Color(217, 83, 79));
    error_panel.setOutlineThickness(2.0f);
    target.draw(error_panel);
    
    // Error icon (simple red circle)
    float icon_radius = 12.0f;
    sf::CircleShape error_icon(icon_radius);
    error_icon.setPosition(panel_x + UIStyle::SPACING_MD, 
                          panel_y + (panel_height - icon_radius * 2) / 2.0f);
    error_icon.setFillColor(sf::Color::White);
    target.draw(error_icon);
    
    // Note: Text rendering would require font, which is handled by UIStyle system
    // For now, the visual indicator (red panel + icon) is sufficient
    // Full text rendering can be added when font system is integrated
}

} // namespace ui
} // namespace reversi

