/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Network Lobby State (State Pattern Extension)
 * Handles room creation and joining for Peer-to-Peer multiplayer
 */

#pragma once

#include "MenuSystem.hpp"
#include "../network/NetworkGame.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <array>

namespace reversi {
namespace ui {

/**
 * @brief Network lobby state
 * 
 * Implements UI/UX design specification:
 * - Create Room: Generate 6-character code, display prominently
 * - Join Room: 6 input boxes, auto-advance, paste support
 * - Network status indicator (🟢🟡🔴)
 * - Connection status display
 */
class NetworkLobbyState : public MenuState {
public:
    /**
     * @brief Lobby mode
     */
    enum class Mode {
        CREATE_ROOM,    // Creating room (Host)
        JOIN_ROOM       // Joining room (Client)
    };
    
    /**
     * @brief Connection result
     */
    struct ConnectionResult {
        bool success;
        std::string room_code;
        std::string host_ip;
        unsigned short host_port;
        network::NetworkRole role;
        
        ConnectionResult() : success(false), host_port(0), 
                            role(network::NetworkRole::HOST) {}
    };

public:
    /**
     * @brief Constructor
     * @param mode Create or Join mode
     */
    explicit NetworkLobbyState(Mode mode);
    
    ~NetworkLobbyState() override;
    
    void update(float dt) override;
    void render(sf::RenderTarget& target) override;
    bool handle_event(const sf::Event& event) override;
    std::unique_ptr<MenuState> get_next_state() override;
    bool is_finished() const override;
    
    /**
     * @brief Get connection result
     */
    ConnectionResult get_connection_result() const { return connection_result_; }

private:
    Mode mode_;
    ConnectionResult connection_result_;
    
    // UI elements
    std::unique_ptr<sf::Font> font_;
    std::vector<std::unique_ptr<UIComponent>> components_;
    
    // Create Room UI
    std::string room_code_;
    bool room_created_;
    std::chrono::steady_clock::time_point room_created_time_;
    
    // Join Room UI
    std::array<char, 6> room_code_input_;
    int current_input_box_;
    bool all_boxes_filled_;
    
    // Network status
    network::NetworkGameState network_state_;
    uint32_t network_rtt_ms_;
    
    // Status messages
    std::string status_message_;
    float status_animation_time_;
    
    // Helper methods
    void setup_create_room_ui();
    void setup_join_room_ui();
    void update_create_room(float dt);
    void update_join_room(float dt);
    void render_create_room(sf::RenderTarget& target);
    void render_join_room(sf::RenderTarget& target);
    void handle_create_room();
    void handle_join_room();
    void handle_keyboard_input(const sf::Event::KeyEvent& event);
    void handle_text_input(const sf::Event::TextEvent& event);
    void handle_paste(const std::string& text);
    void update_input_boxes();
    std::string get_room_code_from_input() const;
    void set_status_message(const std::string& message);
    sf::Color get_network_status_color() const;
};

} // namespace ui
} // namespace reversi

