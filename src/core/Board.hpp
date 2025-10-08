/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Board Class - Bitboard Implementation
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace reversi {
namespace core {

/**
 * @brief Bitboard-based Reversi board representation
 * 
 * Uses two 64-bit integers to represent player and opponent pieces.
 * Efficient move generation and flipping using bitwise operations.
 */
class Board {
public:
    uint64_t player;    // Bitboard for current player's pieces
    uint64_t opponent;  // Bitboard for opponent's pieces

public:
    // Constructors
    Board();
    Board(uint64_t player, uint64_t opponent);
    Board(const std::string& board_str);
    
    // Copy operations
    Board copy() const;
    void copy(Board* dest) const;
    
    // Move generation (to be implemented in Week 3)
    uint64_t legal_moves() const;
    std::vector<int> get_legal_moves() const;
    
    // Move execution (to be implemented in Week 3)
    void make_move(int pos);
    void undo_move(int pos);
    void pass();
    
    // Board state queries
    int count_player() const;
    int count_opponent() const;
    bool is_terminal() const;
    int get_winner() const;
    
    // Utility functions
    std::string to_string() const;
    void print() const;
    
    // Hash (for transposition table - Week 6)
    uint64_t hash() const;
    
private:
    // Helper functions for move generation (Week 1-2 learning)
    uint64_t calc_legal_impl() const;
    uint64_t calc_flip(int pos) const;
};

} // namespace core
} // namespace reversi

