/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Network Message Types (Strategy Pattern)
 * Defines all network message types for Peer-to-Peer LAN multiplayer
 */

#pragma once

#include <cstdint>
#include <string>

namespace reversi {
namespace network {

/**
 * @brief Network message types
 */
enum class MessageType : uint8_t {
    CONNECT = 0x01,         // Connection request (contains version, player name)
    CONNECT_ACK = 0x02,     // Connection acknowledgment (contains player color, initial board)
    MOVE = 0x10,            // Move message (contains position, board hash)
    MOVE_ACK = 0x11,        // Move acknowledgment (contains verification result)
    SYNC_REQUEST = 0x20,    // Sync request (state mismatch detected)
    SYNC_RESPONSE = 0x21,   // Sync response (complete board state)
    HEARTBEAT = 0x30,       // Heartbeat message (keep connection alive)
    HEARTBEAT_ACK = 0x31,   // Heartbeat acknowledgment (RTT calculation)
    DISCONNECT = 0x40,      // Disconnect message
    ERROR = 0xFF            // Error message (version mismatch, etc.)
};

/**
 * @brief Protocol version for compatibility checking
 */
constexpr uint8_t PROTOCOL_VERSION = 0x01;

/**
 * @brief Network message structure (fixed 82 bytes, binary protocol)
 * 
 * Uses network byte order (big-endian) for multi-byte fields.
 * All fields are serialized using htonl/ntohl conversion.
 */
struct NetworkMessage {
    uint8_t message_type;      // 1 byte - MessageType enum value
    uint8_t protocol_version;  // 1 byte - Protocol version (compatibility check)
    uint16_t sequence_number;  // 2 bytes - Sequence number (order guarantee, network byte order)
    uint32_t timestamp_ms;     // 4 bytes - Timestamp in milliseconds (RTT calculation, network byte order)
    uint64_t board_hash;       // 8 bytes - Board Zobrist hash (using Board::hash(), network byte order)
    uint8_t data[64];          // 64 bytes - Message data (Move position, board state, etc.)
    uint16_t checksum;         // 2 bytes - Checksum for data integrity (network byte order)
    
    // Total: 82 bytes (fixed size for efficient serialization)
    
    /**
     * @brief Default constructor (initializes to zero)
     */
    NetworkMessage() : message_type(0), protocol_version(PROTOCOL_VERSION),
                       sequence_number(0), timestamp_ms(0), board_hash(0),
                       checksum(0) {
        std::fill(data, data + 64, 0);
    }
    
    /**
     * @brief Constructor with message type
     */
    explicit NetworkMessage(MessageType type) : NetworkMessage() {
        message_type = static_cast<uint8_t>(type);
    }
};

/**
 * @brief Helper functions for message type conversion
 */
inline MessageType get_message_type(const NetworkMessage& msg) {
    return static_cast<MessageType>(msg.message_type);
}

inline bool is_valid_message_type(uint8_t type) {
    return type >= 0x01 && type <= 0xFF;
}

/**
 * @brief Error codes for ERROR message type
 */
enum class ErrorCode : uint8_t {
    VERSION_MISMATCH = 0x01,   // Protocol version mismatch
    ROOM_NOT_FOUND = 0x02,     // Room code not found or expired
    ROOM_FULL = 0x03,          // Room is full (already has 2 players)
    INVALID_MOVE = 0x04,       // Invalid move received
    STATE_MISMATCH = 0x05,     // Board state mismatch detected
    CONNECTION_TIMEOUT = 0x06, // Connection timeout
    UNKNOWN_ERROR = 0xFF       // Unknown error
};

} // namespace network
} // namespace reversi

