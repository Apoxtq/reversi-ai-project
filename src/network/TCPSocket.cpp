/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * TCP Socket Implementation
 */

#include "TCPSocket.hpp"
#include "NetworkProtocol.hpp"
#include <sstream>

namespace reversi {
namespace network {

TCPSocket::TCPSocket(Mode mode) 
    : mode_(mode), state_(State::DISCONNECTED), remote_port_(0) {
    if (mode == Mode::HOST) {
        listener_ = std::make_unique<sf::TcpListener>();
    } else {
        socket_ = std::make_unique<sf::TcpSocket>();
    }
}

TCPSocket::~TCPSocket() {
    disconnect();
}

bool TCPSocket::listen(unsigned short port) {
    if (mode_ != Mode::HOST) {
        last_error_ = "listen() can only be called in HOST mode";
        return false;
    }
    
    std::lock_guard<std::mutex> lock(socket_mutex_);
    
    set_state(State::CONNECTING);
    
    sf::Socket::Status status = listener_->listen(port);
    if (status != sf::Socket::Done) {
        set_state(State::ERROR);
        last_error_ = "Failed to start listening";
        return false;
    }
    
    unsigned short local_port = listener_->getLocalPort();
    set_state(State::CONNECTING);  // Waiting for connection
    
    return true;
}

bool TCPSocket::accept(unsigned int timeout_ms) {
    if (mode_ != Mode::HOST) {
        last_error_ = "accept() can only be called in HOST mode";
        return false;
    }
    
    std::lock_guard<std::mutex> lock(socket_mutex_);
    
    if (!listener_) {
        last_error_ = "Listener not initialized";
        return false;
    }
    
    connected_socket_ = std::make_unique<sf::TcpSocket>();
    connected_socket_->setBlocking(timeout_ms > 0);
    
    if (timeout_ms > 0) {
        connected_socket_->setBlocking(true);
    }
    
    sf::Socket::Status status = listener_->accept(*connected_socket_);
    
    if (status == sf::Socket::Done) {
        remote_address_ = connected_socket_->getRemoteAddress().toString();
        remote_port_ = connected_socket_->getRemotePort();
        set_state(State::CONNECTED);
        return true;
    } else if (status == sf::Socket::NotReady && timeout_ms == 0) {
        // Non-blocking, not ready yet
        return false;
    } else {
        set_state(State::ERROR);
        last_error_ = "Failed to accept connection";
        return false;
    }
}

bool TCPSocket::connect(const std::string& address, unsigned short port, 
                        unsigned int timeout_ms) {
    if (mode_ != Mode::CLIENT) {
        last_error_ = "connect() can only be called in CLIENT mode";
        return false;
    }
    
    std::lock_guard<std::mutex> lock(socket_mutex_);
    
    if (!socket_) {
        socket_ = std::make_unique<sf::TcpSocket>();
    }
    
    set_state(State::CONNECTING);
    
    socket_->setBlocking(timeout_ms > 0);
    
    sf::IpAddress ip_address;
    if (!ip_address.fromString(address)) {
        set_state(State::ERROR);
        last_error_ = "Invalid IP address: " + address;
        return false;
    }
    
    sf::Socket::Status status = socket_->connect(ip_address, port, 
                                                 sf::milliseconds(timeout_ms));
    
    if (status == sf::Socket::Done) {
        remote_address_ = address;
        remote_port_ = port;
        set_state(State::CONNECTED);
        return true;
    } else {
        set_state(State::ERROR);
        last_error_ = "Failed to connect to " + address + ":" + std::to_string(port);
        return false;
    }
}

void TCPSocket::disconnect() {
    std::lock_guard<std::mutex> lock(socket_mutex_);
    
    if (connected_socket_) {
        connected_socket_->disconnect();
        connected_socket_.reset();
    }
    
    if (socket_) {
        socket_->disconnect();
    }
    
    if (listener_) {
        listener_->close();
    }
    
    set_state(State::DISCONNECTED);
    remote_address_.clear();
    remote_port_ = 0;
}

bool TCPSocket::send_message(const NetworkMessage& msg) {
    std::lock_guard<std::mutex> lock(socket_mutex_);
    
    if (state_ != State::CONNECTED) {
        last_error_ = "Not connected";
        return false;
    }
    
    sf::TcpSocket* active_socket = get_active_socket();
    if (!active_socket) {
        last_error_ = "No active socket";
        return false;
    }
    
    // Serialize message
    std::vector<uint8_t> buffer;
    if (!NetworkProtocol::serialize(msg, buffer)) {
        last_error_ = "Failed to serialize message";
        return false;
    }
    
    // Send data
    sf::Socket::Status status = active_socket->send(buffer.data(), buffer.size());
    
    if (status == sf::Socket::Done) {
        return true;
    } else if (status == sf::Socket::Disconnected) {
        set_state(State::DISCONNECTED);
        last_error_ = "Connection lost";
        return false;
    } else {
        last_error_ = "Failed to send message";
        return false;
    }
}

bool TCPSocket::receive_message(NetworkMessage& msg) {
    std::lock_guard<std::mutex> lock(socket_mutex_);
    
    if (state_ != State::CONNECTED) {
        return false;
    }
    
    sf::TcpSocket* active_socket = get_active_socket();
    if (!active_socket) {
        return false;
    }
    
    // Check if data is available (non-blocking)
    active_socket->setBlocking(false);
    
    // Try to receive 82 bytes (fixed message size)
    std::vector<uint8_t> buffer(82);
    std::size_t received = 0;
    
    sf::Socket::Status status = active_socket->receive(buffer.data(), 82, received);
    
    if (status == sf::Socket::Done && received == 82) {
        // Deserialize message
        if (NetworkProtocol::deserialize(buffer, msg)) {
            // Verify checksum
            if (NetworkProtocol::verify_checksum(msg)) {
                return true;
            } else {
                last_error_ = "Checksum verification failed";
                return false;
            }
        } else {
            last_error_ = "Failed to deserialize message";
            return false;
        }
    } else if (status == sf::Socket::Disconnected) {
        set_state(State::DISCONNECTED);
        last_error_ = "Connection lost";
        return false;
    } else {
        // Not ready or partial data
        return false;
    }
}

bool TCPSocket::is_connected() const {
    return state_ == State::CONNECTED;
}

TCPSocket::State TCPSocket::get_state() const {
    return state_;
}

unsigned short TCPSocket::get_local_port() const {
    std::lock_guard<std::mutex> lock(socket_mutex_);
    
    if (mode_ == Mode::HOST && listener_) {
        return listener_->getLocalPort();
    } else if (mode_ == Mode::CLIENT && socket_) {
        return socket_->getLocalPort();
    }
    return 0;
}

std::string TCPSocket::get_remote_address() const {
    return remote_address_;
}

unsigned short TCPSocket::get_remote_port() const {
    return remote_port_;
}

std::string TCPSocket::get_last_error() const {
    return last_error_;
}

sf::TcpSocket* TCPSocket::get_active_socket() {
    if (mode_ == Mode::HOST) {
        return connected_socket_.get();
    } else {
        return socket_.get();
    }
}

const sf::TcpSocket* TCPSocket::get_active_socket() const {
    if (mode_ == Mode::HOST) {
        return connected_socket_.get();
    } else {
        return socket_.get();
    }
}

void TCPSocket::set_state(State state) {
    state_ = state;
}

} // namespace network
} // namespace reversi

