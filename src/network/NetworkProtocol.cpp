/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Network Protocol Implementation
 */

#include "NetworkProtocol.hpp"
#include <algorithm>
#include <chrono>
#include <cstring>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#undef ERROR  // Undefine Windows ERROR macro to avoid conflict with MessageType::ERROR
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

namespace reversi {
namespace network {

bool NetworkProtocol::serialize(const NetworkMessage& msg, std::vector<uint8_t>& buffer) {
    buffer.resize(82);  // Fixed size: 82 bytes
    size_t offset = 0;
    
    // message_type (1 byte)
    buffer[offset++] = msg.message_type;
    
    // protocol_version (1 byte)
    buffer[offset++] = msg.protocol_version;
    
    // sequence_number (2 bytes, network byte order)
    uint16_t seq_net = htons(msg.sequence_number);
    std::memcpy(buffer.data() + offset, &seq_net, 2);
    offset += 2;
    
    // timestamp_ms (4 bytes, network byte order)
    uint32_t ts_net = htonl(msg.timestamp_ms);
    std::memcpy(buffer.data() + offset, &ts_net, 4);
    offset += 4;
    
    // board_hash (8 bytes, network byte order)
    // Split 64-bit into two 32-bit values for byte order conversion
    uint32_t hash_low = static_cast<uint32_t>(msg.board_hash & 0xFFFFFFFFULL);
    uint32_t hash_high = static_cast<uint32_t>((msg.board_hash >> 32) & 0xFFFFFFFFULL);
    uint32_t hash_low_net = htonl(hash_low);
    uint32_t hash_high_net = htonl(hash_high);
    std::memcpy(buffer.data() + offset, &hash_high_net, 4);
    offset += 4;
    std::memcpy(buffer.data() + offset, &hash_low_net, 4);
    offset += 4;
    
    // data (64 bytes)
    std::memcpy(buffer.data() + offset, msg.data, 64);
    offset += 64;
    
    // checksum (2 bytes, network byte order)
    uint16_t checksum_net = htons(msg.checksum);
    std::memcpy(buffer.data() + offset, &checksum_net, 2);
    offset += 2;
    
    return offset == 82;
}

bool NetworkProtocol::deserialize(const std::vector<uint8_t>& buffer, NetworkMessage& msg) {
    if (buffer.size() < 82) {
        return false;
    }
    
    size_t offset = 0;
    
    // message_type (1 byte)
    msg.message_type = buffer[offset++];
    
    // protocol_version (1 byte)
    msg.protocol_version = buffer[offset++];
    
    // sequence_number (2 bytes, network byte order)
    uint16_t seq_net;
    std::memcpy(&seq_net, buffer.data() + offset, 2);
    msg.sequence_number = ntohs(seq_net);
    offset += 2;
    
    // timestamp_ms (4 bytes, network byte order)
    uint32_t ts_net;
    std::memcpy(&ts_net, buffer.data() + offset, 4);
    msg.timestamp_ms = ntohl(ts_net);
    offset += 4;
    
    // board_hash (8 bytes, network byte order)
    uint32_t hash_high_net, hash_low_net;
    std::memcpy(&hash_high_net, buffer.data() + offset, 4);
    offset += 4;
    std::memcpy(&hash_low_net, buffer.data() + offset, 4);
    offset += 4;
    uint32_t hash_high = ntohl(hash_high_net);
    uint32_t hash_low = ntohl(hash_low_net);
    msg.board_hash = (static_cast<uint64_t>(hash_high) << 32) | hash_low;
    
    // data (64 bytes)
    std::memcpy(msg.data, buffer.data() + offset, 64);
    offset += 64;
    
    // checksum (2 bytes, network byte order)
    uint16_t checksum_net;
    std::memcpy(&checksum_net, buffer.data() + offset, 2);
    msg.checksum = ntohs(checksum_net);
    offset += 2;
    
    return offset == 82;
}

uint16_t NetworkProtocol::calculate_checksum(const NetworkMessage& msg) {
    // Simple checksum: sum of all bytes except checksum field
    uint32_t sum = 0;
    
    sum += msg.message_type;
    sum += msg.protocol_version;
    sum += (msg.sequence_number & 0xFF) + ((msg.sequence_number >> 8) & 0xFF);
    sum += (msg.timestamp_ms & 0xFF) + ((msg.timestamp_ms >> 8) & 0xFF) +
           ((msg.timestamp_ms >> 16) & 0xFF) + ((msg.timestamp_ms >> 24) & 0xFF);
    
    // Board hash (8 bytes)
    uint64_t hash = msg.board_hash;
    for (int i = 0; i < 8; ++i) {
        sum += (hash & 0xFF);
        hash >>= 8;
    }
    
    // Data (64 bytes)
    for (int i = 0; i < 64; ++i) {
        sum += msg.data[i];
    }
    
    // Return 16-bit checksum
    return static_cast<uint16_t>(sum & 0xFFFF);
}

bool NetworkProtocol::verify_checksum(const NetworkMessage& msg) {
    uint16_t calculated = calculate_checksum(msg);
    return calculated == msg.checksum;
}

NetworkMessage NetworkProtocol::create_move_message(const core::Move& move,
                                                    const core::Board& board,
                                                    uint16_t sequence) {
    NetworkMessage msg(MessageType::MOVE);
    msg.sequence_number = sequence;
    msg.timestamp_ms = get_timestamp_ms();
    msg.board_hash = board.hash();  // Use Board::hash() method
    
    // Store move position in data[0-3] (as 32-bit integer, network byte order)
    int32_t pos = static_cast<int32_t>(move.position);
    uint32_t pos_net = htonl(static_cast<uint32_t>(pos));
    std::memcpy(msg.data, &pos_net, 4);
    
    // Calculate and set checksum
    msg.checksum = calculate_checksum(msg);
    
    return msg;
}

core::Move NetworkProtocol::extract_move(const NetworkMessage& msg) {
    if (get_message_type(msg) != MessageType::MOVE) {
        return core::Move(core::Move::NULL_MOVE);
    }
    
    // Extract position from data[0-3]
    uint32_t pos_net;
    std::memcpy(&pos_net, msg.data, 4);
    int32_t pos = static_cast<int32_t>(ntohl(pos_net));
    
    return core::Move(pos);
}

NetworkMessage NetworkProtocol::create_connect_message(const std::string& player_name,
                                                        uint16_t sequence) {
    NetworkMessage msg(MessageType::CONNECT);
    msg.sequence_number = sequence;
    msg.timestamp_ms = get_timestamp_ms();
    msg.board_hash = 0;  // No board state yet
    
    copy_string_to_data(player_name, msg.data, 32);
    
    msg.checksum = calculate_checksum(msg);
    return msg;
}

NetworkMessage NetworkProtocol::create_connect_ack_message(uint8_t player_color,
                                                            const core::Board& board,
                                                            uint16_t sequence) {
    NetworkMessage msg(MessageType::CONNECT_ACK);
    msg.sequence_number = sequence;
    msg.timestamp_ms = get_timestamp_ms();
    msg.board_hash = board.hash();  // Use Board::hash() method
    
    // Store player color in data[0]
    msg.data[0] = player_color;
    
    // Store board state in data[1-16] (player and opponent bitboards, 8 bytes each)
    uint64_t player_bb = board.player;
    uint64_t opponent_bb = board.opponent;
    
    // Convert to network byte order
    uint32_t player_low = static_cast<uint32_t>(player_bb & 0xFFFFFFFFULL);
    uint32_t player_high = static_cast<uint32_t>((player_bb >> 32) & 0xFFFFFFFFULL);
    uint32_t opponent_low = static_cast<uint32_t>(opponent_bb & 0xFFFFFFFFULL);
    uint32_t opponent_high = static_cast<uint32_t>((opponent_bb >> 32) & 0xFFFFFFFFULL);
    
    std::memcpy(msg.data + 1, &player_high, 4);
    std::memcpy(msg.data + 5, &player_low, 4);
    std::memcpy(msg.data + 9, &opponent_high, 4);
    std::memcpy(msg.data + 13, &opponent_low, 4);
    
    msg.checksum = calculate_checksum(msg);
    return msg;
}

NetworkMessage NetworkProtocol::create_heartbeat_message(uint16_t sequence) {
    NetworkMessage msg(MessageType::HEARTBEAT);
    msg.sequence_number = sequence;
    msg.timestamp_ms = get_timestamp_ms();
    msg.board_hash = 0;
    
    msg.checksum = calculate_checksum(msg);
    return msg;
}

NetworkMessage NetworkProtocol::create_sync_request_message(const core::Board& board,
                                                             uint16_t sequence) {
    NetworkMessage msg(MessageType::SYNC_REQUEST);
    msg.sequence_number = sequence;
    msg.timestamp_ms = get_timestamp_ms();
    msg.board_hash = board.hash();  // Use Board::hash() method
    
    msg.checksum = calculate_checksum(msg);
    return msg;
}

NetworkMessage NetworkProtocol::create_error_message(ErrorCode error_code,
                                                      const std::string& error_message,
                                                      uint16_t sequence) {
    NetworkMessage msg(static_cast<MessageType>(0xFF));  // ERROR = 0xFF
    msg.sequence_number = sequence;
    msg.timestamp_ms = get_timestamp_ms();
    msg.board_hash = 0;
    
    // Store error code in data[0]
    msg.data[0] = static_cast<uint8_t>(error_code);
    
    // Store error message in data[1-32]
    copy_string_to_data(error_message, msg.data + 1, 32);
    
    msg.checksum = calculate_checksum(msg);
    return msg;
}

uint32_t NetworkProtocol::get_timestamp_ms() {
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    return static_cast<uint32_t>(milliseconds.count());
}

void NetworkProtocol::copy_string_to_data(const std::string& str, uint8_t* data, size_t max_len) {
    size_t len = std::min(str.length(), max_len - 1);
    std::memcpy(data, str.c_str(), len);
    data[len] = '\0';  // Null termination
    // Fill remaining bytes with zeros
    if (len < max_len - 1) {
        std::memset(data + len + 1, 0, max_len - len - 1);
    }
}

std::string NetworkProtocol::extract_string_from_data(const uint8_t* data, size_t max_len) {
    // Find null terminator
    size_t len = 0;
    while (len < max_len && data[len] != '\0') {
        ++len;
    }
    return std::string(reinterpret_cast<const char*>(data), len);
}

} // namespace network
} // namespace reversi

