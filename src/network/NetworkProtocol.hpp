/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Network Protocol - Binary Serialization/Deserialization
 * Handles conversion between NetworkMessage and binary data (network byte order)
 */

#pragma once

#include "NetworkMessage.hpp"
#include "../core/Move.hpp"
#include "../core/Board.hpp"
#include <vector>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

namespace reversi {
namespace network {

/**
 * @brief Network protocol handler
 * 
 * Responsibilities:
 * - Serialize NetworkMessage to binary (network byte order)
 * - Deserialize binary to NetworkMessage (host byte order)
 * - Calculate checksum for data integrity
 * - Handle Move serialization/deserialization
 */
class NetworkProtocol {
public:
    /**
     * @brief Serialize NetworkMessage to binary buffer
     * @param msg Message to serialize
     * @param buffer Output buffer (will be resized to 82 bytes)
     * @return true if successful
     */
    static bool serialize(const NetworkMessage& msg, std::vector<uint8_t>& buffer);
    
    /**
     * @brief Deserialize binary buffer to NetworkMessage
     * @param buffer Input buffer (must be at least 82 bytes)
     * @param msg Output message
     * @return true if successful
     */
    static bool deserialize(const std::vector<uint8_t>& buffer, NetworkMessage& msg);
    
    /**
     * @brief Calculate checksum for message
     * @param msg Message to calculate checksum for
     * @return 16-bit checksum
     */
    static uint16_t calculate_checksum(const NetworkMessage& msg);
    
    /**
     * @brief Verify message checksum
     * @param msg Message to verify
     * @return true if checksum is valid
     */
    static bool verify_checksum(const NetworkMessage& msg);
    
    /**
     * @brief Create MOVE message from Move and Board
     * @param move Move to send
     * @param board Board state (for hash verification)
     * @param sequence Sequence number
     * @return NetworkMessage
     */
    static NetworkMessage create_move_message(const core::Move& move,
                                              const core::Board& board,
                                              uint16_t sequence);
    
    /**
     * @brief Extract Move from MOVE message
     * @param msg MOVE message
     * @return Move (or NULL_MOVE if invalid)
     */
    static core::Move extract_move(const NetworkMessage& msg);
    
    /**
     * @brief Create CONNECT message
     * @param player_name Player name (max 32 chars)
     * @param sequence Sequence number
     * @return NetworkMessage
     */
    static NetworkMessage create_connect_message(const std::string& player_name,
                                                  uint16_t sequence);
    
    /**
     * @brief Create CONNECT_ACK message
     * @param player_color Player color (0=Black, 1=White)
     * @param board Initial board state
     * @param sequence Sequence number
     * @return NetworkMessage
     */
    static NetworkMessage create_connect_ack_message(uint8_t player_color,
                                                     const core::Board& board,
                                                     uint16_t sequence);
    
    /**
     * @brief Create HEARTBEAT message
     * @param sequence Sequence number
     * @return NetworkMessage
     */
    static NetworkMessage create_heartbeat_message(uint16_t sequence);
    
    /**
     * @brief Create SYNC_REQUEST message
     * @param board Current board state (for hash)
     * @param sequence Sequence number
     * @return NetworkMessage
     */
    static NetworkMessage create_sync_request_message(const core::Board& board,
                                                      uint16_t sequence);
    
    /**
     * @brief Create ERROR message
     * @param error_code Error code
     * @param error_message Error message (max 32 chars)
     * @param sequence Sequence number
     * @return NetworkMessage
     */
    static NetworkMessage create_error_message(ErrorCode error_code,
                                               const std::string& error_message,
                                               uint16_t sequence);
    
private:
    /**
     * @brief Get current timestamp in milliseconds
     * @note This is a public utility function for creating messages
     */
public:
    static uint32_t get_timestamp_ms();
    
    /**
     * @brief Copy string to message data (with null termination)
     */
    static void copy_string_to_data(const std::string& str, uint8_t* data, size_t max_len);
    
    /**
     * @brief Extract string from message data
     */
    static std::string extract_string_from_data(const uint8_t* data, size_t max_len);
};

} // namespace network
} // namespace reversi

