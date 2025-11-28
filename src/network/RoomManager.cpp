/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Room Manager Implementation
 */

#include "RoomManager.hpp"
#include <algorithm>
#include <chrono>

namespace reversi {
namespace network {

RoomManager::RoomManager() 
    : rng_(std::random_device{}()),
      code_dist_(0, VALID_CHARS_COUNT - 1) {
}

RoomManager& RoomManager::get_instance() {
    static RoomManager instance;
    return instance;
}

std::string RoomManager::create_room(const std::string& host_ip, unsigned short host_port) {
    std::lock_guard<std::mutex> lock(rooms_mutex_);
    
    // Clean up expired rooms first
    cleanup_expired_rooms();
    
    // Generate unique room code
    std::string code;
    int attempts = 0;
    const int MAX_ATTEMPTS = 100;
    
    do {
        code = generate_room_code();
        ++attempts;
    } while (rooms_.find(code) != rooms_.end() && attempts < MAX_ATTEMPTS);
    
    if (attempts >= MAX_ATTEMPTS) {
        return "";  // Failed to generate unique code
    }
    
    // Create room
    auto room = std::make_shared<Room>(code, host_ip, host_port);
    rooms_[code] = room;
    
    return code;
}

std::shared_ptr<Room> RoomManager::find_room(const std::string& code) {
    std::lock_guard<std::mutex> lock(rooms_mutex_);
    
    if (!is_valid_room_code(code)) {
        return nullptr;
    }
    
    auto it = rooms_.find(code);
    if (it == rooms_.end()) {
        return nullptr;  // Room not found
    }
    
    auto room = it->second;
    
    // Check if expired (5 minutes)
    auto now = std::chrono::steady_clock::now();
    auto age = std::chrono::duration_cast<std::chrono::minutes>(now - room->created_at);
    
    if (age.count() >= ROOM_TIMEOUT_MINUTES) {
        rooms_.erase(it);
        return nullptr;  // Room expired
    }
    
    return room;
}

void RoomManager::set_room_active(const std::string& code, bool active) {
    std::lock_guard<std::mutex> lock(rooms_mutex_);
    
    auto it = rooms_.find(code);
    if (it != rooms_.end()) {
        it->second->is_active = active;
    }
}

void RoomManager::remove_room(const std::string& code) {
    std::lock_guard<std::mutex> lock(rooms_mutex_);
    rooms_.erase(code);
}

void RoomManager::cleanup_expired_rooms() {
    auto now = std::chrono::steady_clock::now();
    
    auto it = rooms_.begin();
    while (it != rooms_.end()) {
        auto age = std::chrono::duration_cast<std::chrono::minutes>(
            now - it->second->created_at);
        
        if (age.count() >= ROOM_TIMEOUT_MINUTES) {
            it = rooms_.erase(it);
        } else {
            ++it;
        }
    }
}

bool RoomManager::is_valid_room_code(const std::string& code) {
    if (code.length() != ROOM_CODE_LENGTH) {
        return false;
    }
    
    // Check each character is valid
    for (char c : code) {
        // Convert to uppercase
        char upper = std::toupper(c);
        
        // Check if in valid characters
        bool found = false;
        for (int i = 0; i < VALID_CHARS_COUNT; ++i) {
            if (VALID_CHARS[i] == upper) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            return false;
        }
    }
    
    return true;
}

std::string RoomManager::generate_room_code() {
    std::string code;
    code.reserve(ROOM_CODE_LENGTH);
    
    RoomManager& instance = get_instance();
    
    for (int i = 0; i < ROOM_CODE_LENGTH; ++i) {
        int index = instance.code_dist_(instance.rng_);
        code += VALID_CHARS[index];
    }
    
    return code;
}

} // namespace network
} // namespace reversi

