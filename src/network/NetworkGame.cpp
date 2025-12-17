/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Network Game Implementation
 */

#include "NetworkGame.hpp"
#include "RoomManager.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <algorithm>
#include <map>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

namespace reversi {
namespace network {

NetworkGame::NetworkGame(NetworkRole role)
    : role_(role), state_(NetworkGameState::DISCONNECTED),
      next_send_sequence_(1), last_received_sequence_(0),
      current_rtt_ms_(0) {
    socket_ = std::make_unique<TCPSocket>(
        (role == NetworkRole::HOST) ? TCPSocket::Mode::HOST : TCPSocket::Mode::CLIENT);
}

NetworkGame::~NetworkGame() {
    disconnect();
}

std::string NetworkGame::start_as_host(unsigned short port) {
    if (role_ != NetworkRole::HOST) {
        if (error_callback_) {
            error_callback_("start_as_host() can only be called in HOST role");
        }
        return "";
    }
    
    set_state(NetworkGameState::CONNECTING);
    
    // Start listening
    if (!socket_->listen(port)) {
        set_state(NetworkGameState::ERROR_STATE);
        if (error_callback_) {
            error_callback_("Failed to start listening: " + socket_->get_last_error());
        }
        return "";
    }
    
    // Get local IP and port
    std::string local_ip = get_local_ip_address();
    unsigned short local_port = socket_->get_local_port();
    
    // Create room
    room_code_ = RoomManager::get_instance().create_room(local_ip, local_port);
    
    if (room_code_.empty()) {
        set_state(NetworkGameState::ERROR_STATE);
        if (error_callback_) {
            error_callback_("Failed to create room");
        }
        return "";
    }
    
    return room_code_;
}

bool NetworkGame::start_as_client(const std::string& room_code,
                                  const std::string& host_ip,
                                  unsigned short host_port) {
    if (role_ != NetworkRole::CLIENT) {
        if (error_callback_) {
            error_callback_("start_as_client() can only be called in CLIENT role");
        }
        return false;
    }
    
    set_state(NetworkGameState::CONNECTING);
    
    // Connect to host
    if (!socket_->connect(host_ip, host_port, 5000)) {
        set_state(NetworkGameState::ERROR_STATE);
        if (error_callback_) {
            error_callback_("Failed to connect: " + socket_->get_last_error());
        }
        return false;
    }
    
    // Send CONNECT message
    auto connect_msg = NetworkProtocol::create_connect_message("Player", next_send_sequence_++);
    if (!socket_->send_message(connect_msg)) {
        set_state(NetworkGameState::ERROR_STATE);
        if (error_callback_) {
            error_callback_("Failed to send CONNECT message");
        }
        return false;
    }
    
    room_code_ = room_code;
    return true;
}

void NetworkGame::update(float dt) {
    if (state_ == NetworkGameState::DISCONNECTED || 
        state_ == NetworkGameState::ERROR_STATE) {
        return;
    }
    
    // Accept connection (Host mode)
    if (role_ == NetworkRole::HOST && state_ == NetworkGameState::CONNECTING) {
        if (socket_->accept(0)) {  // Non-blocking
            set_state(NetworkGameState::CONNECTED);
        }
    }
    
    // Process received messages
    process_received_messages();
    
    // Send heartbeat
    send_heartbeat();
    
    // Check heartbeat timeout
    check_heartbeat_timeout();
}

bool NetworkGame::send_move(const core::Move& move, const core::Board& board) {
    if (state_ != NetworkGameState::PLAYING) {
        return false;
    }
    
    // Create MOVE message using Board::hash()
    auto msg = NetworkProtocol::create_move_message(move, board, next_send_sequence_++);
    
    if (!socket_->send_message(msg)) {
        if (error_callback_) {
            error_callback_("Failed to send move: " + socket_->get_last_error());
        }
        return false;
    }
    
    return true;
}

bool NetworkGame::is_my_turn() const {
    // This depends on game logic, not network state
    // For now, assume Host (Black) goes first
    // This should be managed by GameState, not NetworkGame
    return true;  // Placeholder
}

void NetworkGame::disconnect() {
    if (socket_ && socket_->is_connected()) {
        // Send DISCONNECT message
        NetworkMessage disconnect_msg(MessageType::DISCONNECT);
        disconnect_msg.sequence_number = next_send_sequence_++;
        disconnect_msg.timestamp_ms = NetworkProtocol::get_timestamp_ms();
        socket_->send_message(disconnect_msg);
    }
    
    socket_->disconnect();
    
    if (!room_code_.empty()) {
        RoomManager::get_instance().remove_room(room_code_);
        room_code_.clear();
    }
    
    set_state(NetworkGameState::DISCONNECTED);
}

void NetworkGame::request_sync(uint64_t expected_hash) {
    if (state_ != NetworkGameState::PLAYING) {
        return;
    }
    
    // Create SYNC_REQUEST message
    NetworkMessage sync_msg(MessageType::SYNC_REQUEST);
    sync_msg.sequence_number = next_send_sequence_++;
    sync_msg.timestamp_ms = NetworkProtocol::get_timestamp_ms();
    sync_msg.board_hash = expected_hash;
    sync_msg.checksum = NetworkProtocol::calculate_checksum(sync_msg);
    
    socket_->send_message(sync_msg);
    set_state(NetworkGameState::SYNCING);
}

void NetworkGame::set_state(NetworkGameState new_state) {
    if (state_ != new_state) {
        state_ = new_state;
        if (state_changed_callback_) {
            state_changed_callback_(new_state);
        }
    }
}

void NetworkGame::process_received_messages() {
    NetworkMessage msg;
    while (socket_->receive_message(msg)) {
        received_messages_.push(msg);
    }
    
    // Process queued messages
    while (!received_messages_.empty()) {
        msg = received_messages_.front();
        received_messages_.pop();
        
        // Verify checksum
        if (!NetworkProtocol::verify_checksum(msg)) {
            if (error_callback_) {
                error_callback_("Checksum verification failed");
            }
            continue;
        }
        
        // Update last received sequence
        if (msg.sequence_number > last_received_sequence_) {
            last_received_sequence_ = msg.sequence_number;
        }
        
        // Handle message by type
        MessageType type = get_message_type(msg);
        switch (type) {
            case MessageType::CONNECT:
                handle_connect_message(msg);
                break;
            case MessageType::CONNECT_ACK:
                // Connection acknowledged, start game
                set_state(NetworkGameState::PLAYING);
                break;
            case MessageType::MOVE:
                handle_move_message(msg);
                break;
            case MessageType::MOVE_ACK:
                // Move acknowledged (no action needed for now)
                break;
            case MessageType::SYNC_REQUEST:
                handle_sync_request(msg);
                break;
            case MessageType::HEARTBEAT:
                // Send HEARTBEAT_ACK
                {
                    NetworkMessage ack(MessageType::HEARTBEAT_ACK);
                    ack.sequence_number = msg.sequence_number;
                    ack.timestamp_ms = NetworkProtocol::get_timestamp_ms();
                    ack.checksum = NetworkProtocol::calculate_checksum(ack);
                    socket_->send_message(ack);
                }
                last_heartbeat_received_ = std::chrono::steady_clock::now();
                break;
            case MessageType::HEARTBEAT_ACK:
                // Calculate RTT
                {
                    auto it = heartbeat_send_times_.find(msg.sequence_number);
                    if (it != heartbeat_send_times_.end()) {
                        auto now = std::chrono::steady_clock::now();
                        auto rtt = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now - it->second);
                        current_rtt_ms_ = static_cast<uint32_t>(rtt.count());
                        heartbeat_send_times_.erase(it);
                    }
                }
                break;
            case MessageType::DISCONNECT:
                set_state(NetworkGameState::DISCONNECTED);
                break;
            case MessageType::ERROR:  // ERROR = 0xFF
                handle_error_message(msg);
                break;
            default:
                break;
        }
    }
}

void NetworkGame::send_heartbeat() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - last_heartbeat_sent_).count() / 1000.0f;
    
    if (elapsed >= HEARTBEAT_INTERVAL && socket_->is_connected()) {
        auto msg = NetworkProtocol::create_heartbeat_message(next_send_sequence_++);
        heartbeat_send_times_[msg.sequence_number] = now;
        socket_->send_message(msg);
        last_heartbeat_sent_ = now;
    }
}

void NetworkGame::check_heartbeat_timeout() {
    if (!socket_->is_connected()) {
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - last_heartbeat_received_).count() / 1000.0f;
    
    if (elapsed >= HEARTBEAT_TIMEOUT) {
        set_state(NetworkGameState::ERROR_STATE);
        if (error_callback_) {
            error_callback_("Heartbeat timeout - connection lost");
        }
    }
}

void NetworkGame::handle_connect_message(const NetworkMessage& msg) {
    // Host receives CONNECT from Client
    if (role_ == NetworkRole::HOST && state_ == NetworkGameState::CONNECTED) {
        // Send CONNECT_ACK with player color and initial board
        // Note: Board should be provided by GameState, not created here
        // For now, create default board
        core::Board initial_board;
        auto ack = NetworkProtocol::create_connect_ack_message(
            get_player_color(), initial_board, next_send_sequence_++);
        socket_->send_message(ack);
        set_state(NetworkGameState::PLAYING);
    }
}

void NetworkGame::handle_move_message(const NetworkMessage& msg) {
    if (state_ != NetworkGameState::PLAYING) {
        return;
    }
    
    // Extract move
    core::Move move = NetworkProtocol::extract_move(msg);
    
    // Verify board hash (using received hash)
    uint64_t received_hash = msg.board_hash;
    
    // Send MOVE_ACK
    NetworkMessage ack(MessageType::MOVE_ACK);
    ack.sequence_number = msg.sequence_number;
    ack.timestamp_ms = NetworkProtocol::get_timestamp_ms();
    ack.board_hash = received_hash;
    ack.checksum = NetworkProtocol::calculate_checksum(ack);
    socket_->send_message(ack);
    
    // Callback with move and hash for verification
    if (move_received_callback_) {
        move_received_callback_(move, received_hash);
    }
}

void NetworkGame::handle_sync_request(const NetworkMessage& msg) {
    // Opponent requested sync
    if (sync_request_callback_) {
        sync_request_callback_(msg.board_hash);
    }
}

void NetworkGame::handle_error_message(const NetworkMessage& msg) {
    ErrorCode error_code = static_cast<ErrorCode>(msg.data[0]);
    std::string error_msg = NetworkProtocol::extract_string_from_data(msg.data + 1, 32);
    
    set_state(NetworkGameState::ERROR_STATE);
    if (error_callback_) {
        error_callback_(error_msg);
    }
}

std::string NetworkGame::get_local_ip_address() {
    // Simplified IP detection - use SFML to get local address
    // For production, consider more robust network interface enumeration
    
    // Try to connect to a remote address to determine local IP
    // This is a common technique
    sf::TcpSocket test_socket;
    test_socket.setBlocking(false);
    
    // Try connecting to a public DNS server (non-blocking, will fail but gives us local IP)
    sf::IpAddress google_dns("8.8.8.8");
    test_socket.connect(google_dns, 80, sf::milliseconds(1));
    
    // Get local address - SFML 2.5 doesn't have getLocalAddress()
    // Use a workaround: connect to a known address and check local endpoint
    test_socket.disconnect();
    
    // Try to get local IP by connecting to a dummy address
    // In SFML 2.5, we need to use a different approach
    // For now, return localhost as fallback
    sf::IpAddress local = sf::IpAddress::LocalHost;
    if (local != sf::IpAddress::None) {
        return local.toString();
    }
    
    // Fallback: return localhost (for testing on same machine)
    return "127.0.0.1";
}

} // namespace network
} // namespace reversi

