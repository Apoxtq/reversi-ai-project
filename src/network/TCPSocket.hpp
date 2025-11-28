/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * TCP Socket Wrapper (Adapter Pattern)
 * Encapsulates SFML Network for Peer-to-Peer communication
 * Supports both Host (TcpListener) and Client (TcpSocket) modes
 */

#pragma once

#include <SFML/Network.hpp>
#include "NetworkMessage.hpp"
#include <vector>
#include <string>
#include <memory>
#include <mutex>

namespace reversi {
namespace network {

/**
 * @brief TCP Socket wrapper for Peer-to-Peer communication
 * 
 * Supports two modes:
 * - Host mode: Uses TcpListener to accept incoming connections
 * - Client mode: Uses TcpSocket to connect to host
 * 
 * Thread-safe for concurrent send/receive operations.
 */
class TCPSocket {
public:
    /**
     * @brief Connection state
     */
    enum class State {
        DISCONNECTED,   // Not connected
        CONNECTING,     // Connection in progress
        CONNECTED,      // Connected and ready
        ERROR           // Connection error
    };
    
    /**
     * @brief Socket mode
     */
    enum class Mode {
        HOST,           // Host mode (listening for connections)
        CLIENT          // Client mode (connecting to host)
    };

private:
    Mode mode_;
    State state_;
    
    // Host mode: listener
    std::unique_ptr<sf::TcpListener> listener_;
    
    // Client mode: socket
    std::unique_ptr<sf::TcpSocket> socket_;
    
    // Connected socket (for host mode after accepting connection)
    std::unique_ptr<sf::TcpSocket> connected_socket_;
    
    // Thread safety
    mutable std::mutex socket_mutex_;
    
    // Connection info
    std::string remote_address_;
    unsigned short remote_port_;
    
public:
    /**
     * @brief Constructor
     * @param mode Host or Client mode
     */
    explicit TCPSocket(Mode mode);
    
    /**
     * @brief Destructor (closes connection)
     */
    ~TCPSocket();
    
    // Non-copyable
    TCPSocket(const TCPSocket&) = delete;
    TCPSocket& operator=(const TCPSocket&) = delete;
    
    // Movable
    TCPSocket(TCPSocket&&) = default;
    TCPSocket& operator=(TCPSocket&&) = default;
    
    /**
     * @brief Start listening (Host mode only)
     * @param port Port to listen on (0 = auto-assign)
     * @return true if successful
     */
    bool listen(unsigned short port = 0);
    
    /**
     * @brief Accept incoming connection (Host mode only)
     * @param timeout_ms Timeout in milliseconds (0 = non-blocking)
     * @return true if connection accepted
     */
    bool accept(unsigned int timeout_ms = 0);
    
    /**
     * @brief Connect to host (Client mode only)
     * @param address Host IP address
     * @param port Host port
     * @param timeout_ms Timeout in milliseconds
     * @return true if connected
     */
    bool connect(const std::string& address, unsigned short port, 
                 unsigned int timeout_ms = 5000);
    
    /**
     * @brief Disconnect
     */
    void disconnect();
    
    /**
     * @brief Send message (non-blocking)
     * @param msg Message to send
     * @return true if sent successfully
     */
    bool send_message(const NetworkMessage& msg);
    
    /**
     * @brief Receive message (non-blocking, polling mode)
     * @param msg Received message
     * @return true if message received
     */
    bool receive_message(NetworkMessage& msg);
    
    /**
     * @brief Check if connected
     */
    bool is_connected() const;
    
    /**
     * @brief Get connection state
     */
    State get_state() const;
    
    /**
     * @brief Get local port (Host mode)
     */
    unsigned short get_local_port() const;
    
    /**
     * @brief Get remote address
     */
    std::string get_remote_address() const;
    
    /**
     * @brief Get remote port
     */
    unsigned short get_remote_port() const;
    
    /**
     * @brief Get socket mode
     */
    Mode get_mode() const { return mode_; }
    
    /**
     * @brief Get last error message
     */
    std::string get_last_error() const;
    
private:
    /**
     * @brief Get active socket (for send/receive)
     */
    sf::TcpSocket* get_active_socket();
    const sf::TcpSocket* get_active_socket() const;
    
    /**
     * @brief Update state
     */
    void set_state(State state);
    
    /**
     * @brief Error message storage
     */
    mutable std::string last_error_;
};

} // namespace network
} // namespace reversi

