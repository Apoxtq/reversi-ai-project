/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Network Game Session (State Pattern, Peer-to-Peer Architecture)
 * Manages network game flow: connection, move synchronization, state verification
 */

#pragma once

#include "TCPSocket.hpp"
#include "NetworkProtocol.hpp"
#include "RoomManager.hpp"
#include "../core/Board.hpp"
#include "../core/Move.hpp"
#include <memory>
#include <string>
#include <functional>
#include <chrono>
#include <queue>

namespace reversi {
namespace network {

/**
 * @brief Network game connection state
 */
enum class NetworkGameState {
    DISCONNECTED,   // Not connected
    CONNECTING,     // Connection in progress
    CONNECTED,      // Connected, waiting for game start
    PLAYING,        // Game in progress
    SYNCING,        // State synchronization in progress
    ERROR           // Error state
};

/**
 * @brief Network game role
 */
enum class NetworkRole {
    HOST,           // Host (creates room, listens for connection)
    CLIENT          // Client (joins room, connects to host)
};

/**
 * @brief Network game session
 * 
 * Responsibilities:
 * - Manage Peer-to-Peer connection (Host or Client)
 * - Synchronize moves between players
 * - Verify board state consistency (using Board::hash())
 * - Handle heartbeat and connection monitoring
 * - Manage reconnection logic
 */
class NetworkGame {
public:
    /**
     * @brief Callback types
     */
    using MoveReceivedCallback = std::function<void(const core::Move&, uint64_t board_hash)>;
    using StateChangedCallback = std::function<void(NetworkGameState)>;
    using ErrorCallback = std::function<void(const std::string&)>;
    using SyncRequestCallback = std::function<void(uint64_t expected_hash)>;

public:
    /**
     * @brief Constructor
     * @param role Host or Client role
     */
    explicit NetworkGame(NetworkRole role);
    
    /**
     * @brief Destructor
     */
    ~NetworkGame();
    
    // Non-copyable
    NetworkGame(const NetworkGame&) = delete;
    NetworkGame& operator=(const NetworkGame&) = delete;
    
    /**
     * @brief Start as Host (create room and listen)
     * @param port Port to listen on (0 = auto-assign)
     * @return Room code if successful, empty string on failure
     */
    std::string start_as_host(unsigned short port = 0);
    
    /**
     * @brief Start as Client (join room)
     * @param room_code Room code
     * @param host_ip Host IP address
     * @param host_port Host port
     * @return true if connection initiated
     */
    bool start_as_client(const std::string& room_code, 
                        const std::string& host_ip, 
                        unsigned short host_port);
    
    /**
     * @brief Update network game (call in game loop)
     * @param dt Delta time in seconds
     */
    void update(float dt);
    
    /**
     * @brief Send move to opponent
     * @param move Move to send
     * @param board Current board state (for hash verification)
     * @return true if sent successfully
     */
    bool send_move(const core::Move& move, const core::Board& board);
    
    /**
     * @brief Check if it's our turn (based on role and game state)
     */
    bool is_my_turn() const;
    
    /**
     * @brief Get current game state
     */
    NetworkGameState get_state() const { return state_; }
    
    /**
     * @brief Get network role
     */
    NetworkRole get_role() const { return role_; }
    
    /**
     * @brief Get current RTT (Round-Trip Time) in milliseconds
     */
    uint32_t get_rtt_ms() const { return current_rtt_ms_; }
    
    /**
     * @brief Get connection status
     */
    bool is_connected() const { return state_ == NetworkGameState::PLAYING || 
                                       state_ == NetworkGameState::CONNECTED; }
    
    /**
     * @brief Disconnect
     */
    void disconnect();
    
    /**
     * @brief Request state synchronization
     * @param expected_hash Expected board hash
     */
    void request_sync(uint64_t expected_hash);
    
    /**
     * @brief Set callbacks
     */
    void set_move_received_callback(MoveReceivedCallback callback) {
        move_received_callback_ = callback;
    }
    
    void set_state_changed_callback(StateChangedCallback callback) {
        state_changed_callback_ = callback;
    }
    
    void set_error_callback(ErrorCallback callback) {
        error_callback_ = callback;
    }
    
    void set_sync_request_callback(SyncRequestCallback callback) {
        sync_request_callback_ = callback;
    }
    
    /**
     * @brief Get player color (0=Black, 1=White)
     * Host is always Black, Client is always White
     */
    uint8_t get_player_color() const {
        return (role_ == NetworkRole::HOST) ? 0 : 1;
    }

private:
    NetworkRole role_;
    NetworkGameState state_;
    std::unique_ptr<TCPSocket> socket_;
    
    // Room information (for Host)
    std::string room_code_;
    
    // Sequence numbers
    uint16_t next_send_sequence_;
    uint16_t last_received_sequence_;
    
    // Heartbeat
    std::chrono::steady_clock::time_point last_heartbeat_sent_;
    std::chrono::steady_clock::time_point last_heartbeat_received_;
    static constexpr float HEARTBEAT_INTERVAL = 2.0f;  // seconds
    static constexpr float HEARTBEAT_TIMEOUT = 5.0f;   // seconds
    
    // RTT calculation
    std::map<uint16_t, std::chrono::steady_clock::time_point> heartbeat_send_times_;
    uint32_t current_rtt_ms_;
    
    // Callbacks
    MoveReceivedCallback move_received_callback_;
    StateChangedCallback state_changed_callback_;
    ErrorCallback error_callback_;
    SyncRequestCallback sync_request_callback_;
    
    // Message queue (for processing received messages)
    std::queue<NetworkMessage> received_messages_;
    
    // Helper methods
    void set_state(NetworkGameState new_state);
    void process_received_messages();
    void send_heartbeat();
    void check_heartbeat_timeout();
    void handle_connect_message(const NetworkMessage& msg);
    void handle_move_message(const NetworkMessage& msg);
    void handle_sync_request(const NetworkMessage& msg);
    void handle_error_message(const NetworkMessage& msg);
    std::string get_local_ip_address();
};

} // namespace network
} // namespace reversi

