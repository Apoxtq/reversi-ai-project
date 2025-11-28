/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Room Manager (Singleton Pattern, Thread-Safe)
 * Manages room codes for Peer-to-Peer LAN multiplayer
 * Note: This is a local lookup table, not a central server
 */

#pragma once

#include <string>
#include <map>
#include <mutex>
#include <chrono>
#include <random>

namespace reversi {
namespace network {

/**
 * @brief Room information
 */
struct Room {
    std::string code;              // 6-character room code
    std::string host_ip;           // Host IP address
    unsigned short host_port;      // Host port
    std::chrono::steady_clock::time_point created_at;  // Creation time
    bool is_active;                // Whether room is active (has 2 players)
    
    Room() : host_port(0), is_active(false) {}
    Room(const std::string& c, const std::string& ip, unsigned short port)
        : code(c), host_ip(ip), host_port(port), is_active(false) {
        created_at = std::chrono::steady_clock::now();
    }
};

/**
 * @brief Room Manager (Singleton, Thread-Safe)
 * 
 * Responsibilities:
 * - Generate unique 6-character room codes
 * - Store room information (Host IP, port)
 * - Clean up expired rooms (5 minutes timeout)
 * - Thread-safe operations
 */
class RoomManager {
public:
    /**
     * @brief Get singleton instance
     */
    static RoomManager& get_instance();
    
    /**
     * @brief Create a new room
     * @param host_ip Host IP address
     * @param host_port Host port
     * @return Room code (6 characters), or empty string on failure
     */
    std::string create_room(const std::string& host_ip, unsigned short host_port);
    
    /**
     * @brief Find room by code
     * @param code Room code
     * @return Room information, or nullptr if not found/expired
     */
    std::shared_ptr<Room> find_room(const std::string& code);
    
    /**
     * @brief Mark room as active (2 players connected)
     */
    void set_room_active(const std::string& code, bool active);
    
    /**
     * @brief Remove room
     */
    void remove_room(const std::string& code);
    
    /**
     * @brief Clean up expired rooms (older than 5 minutes)
     */
    void cleanup_expired_rooms();
    
    /**
     * @brief Check if room code is valid format
     */
    static bool is_valid_room_code(const std::string& code);
    
    /**
     * @brief Generate random room code
     */
    static std::string generate_room_code();

private:
    // Singleton: private constructor
    RoomManager();
    ~RoomManager() = default;
    
    // Non-copyable, non-movable
    RoomManager(const RoomManager&) = delete;
    RoomManager& operator=(const RoomManager&) = delete;
    RoomManager(RoomManager&&) = delete;
    RoomManager& operator=(RoomManager&&) = delete;
    
    // Room storage
    std::map<std::string, std::shared_ptr<Room>> rooms_;
    std::mutex rooms_mutex_;  // Thread safety
    
    // Random number generator for room codes
    std::mt19937 rng_;
    std::uniform_int_distribution<int> code_dist_;
    
    // Constants
    static constexpr int ROOM_CODE_LENGTH = 6;
    static constexpr int ROOM_TIMEOUT_MINUTES = 5;
    // Valid characters: A-Z excluding O/I/L, 0-9 excluding 0/1
    // A-Z (26) - O/I/L (3) = 23 letters
    // 0-9 (10) - 0/1 (2) = 8 digits
    // Total: 23 + 8 = 31 characters
    static constexpr const char* VALID_CHARS = "ABCDEFGHJKMNPQRSTUVWXYZ23456789";
    static constexpr int VALID_CHARS_COUNT = 31;
};

} // namespace network
} // namespace reversi

