/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Move Class Definition
 */

#pragma once

#include <cstdint>
#include <string>

namespace reversi {
namespace core {

/**
 * @brief Represents a move in Reversi
 * 
 * Move is represented by a position (0-63) or special values:
 * - PASS (-1): Player has no legal moves
 * - NULL_MOVE (-2): Invalid/uninitialized move
 */
class Move {
public:
    static constexpr int PASS = -1;
    static constexpr int NULL_MOVE = -2;
    
    int position;  // 0-63 for board positions, PASS or NULL_MOVE
    
public:
    // Constructors
    Move() : position(NULL_MOVE) {}
    explicit Move(int pos) : position(pos) {}
    
    // Queries
    bool is_pass() const { return position == PASS; }
    bool is_null() const { return position == NULL_MOVE; }
    bool is_valid() const { return position >= 0 && position < 64; }
    
    // Conversion
    std::string to_string() const;
    static Move from_string(const std::string& str);
    
    // Operators
    bool operator==(const Move& other) const { 
        return position == other.position; 
    }
    bool operator!=(const Move& other) const { 
        return position != other.position; 
    }
};

} // namespace core
} // namespace reversi

