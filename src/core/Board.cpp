/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Board Implementation - Bitboard Operations
 */

#include "Board.hpp"
#include <bit>
#include <iostream>
#include <sstream>

namespace reversi {
namespace core {

// Standard initial position for Reversi
static const uint64_t INITIAL_PLAYER = 0x0000000810000000ULL;   // White: d4, e5
static const uint64_t INITIAL_OPPONENT = 0x0000001008000000ULL; // Black: d5, e4

Board::Board() 
    : player(INITIAL_PLAYER), opponent(INITIAL_OPPONENT) {
}

Board::Board(uint64_t p, uint64_t o) 
    : player(p), opponent(o) {
}

Board::Board(const std::string& board_str) {
    // TODO: Parse string representation (Week 2)
    player = INITIAL_PLAYER;
    opponent = INITIAL_OPPONENT;
}

Board Board::copy() const {
    return Board(player, opponent);
}

void Board::copy(Board* dest) const {
    dest->player = player;
    dest->opponent = opponent;
}

int Board::count_player() const {
    return std::popcount(player);
}

int Board::count_opponent() const {
    return std::popcount(opponent);
}

uint64_t Board::legal_moves() const {
    // TODO: Implement legal move generation (Week 1-2)
    return 0ULL;
}

std::vector<int> Board::get_legal_moves() const {
    std::vector<int> moves;
    uint64_t legal = legal_moves();
    
    // Extract positions from bitboard
    while (legal) {
        int pos = std::countr_zero(legal);
        moves.push_back(pos);
        legal &= legal - 1; // Clear lowest bit
    }
    
    return moves;
}

void Board::make_move(int pos) {
    // TODO: Implement move execution with flipping (Week 2-3)
}

void Board::undo_move(int pos) {
    // TODO: Implement move undo (Week 3)
}

void Board::pass() {
    // Swap player and opponent
    std::swap(player, opponent);
}

bool Board::is_terminal() const {
    // Game is over if neither player has legal moves
    // TODO: Implement properly (Week 3)
    return false;
}

int Board::get_winner() const {
    int p_count = count_player();
    int o_count = count_opponent();
    
    if (p_count > o_count) return 1;   // Player wins
    if (o_count > p_count) return -1;  // Opponent wins
    return 0;                          // Draw
}

std::string Board::to_string() const {
    std::ostringstream oss;
    oss << "  A B C D E F G H\n";
    
    for (int row = 0; row < 8; ++row) {
        oss << (8 - row) << " ";
        for (int col = 0; col < 8; ++col) {
            int pos = row * 8 + col;
            uint64_t mask = 1ULL << pos;
            
            if (player & mask) {
                oss << "O ";  // Player piece
            } else if (opponent & mask) {
                oss << "X ";  // Opponent piece
            } else {
                oss << ". ";  // Empty
            }
        }
        oss << (8 - row) << "\n";
    }
    oss << "  A B C D E F G H\n";
    
    return oss.str();
}

void Board::print() const {
    std::cout << to_string();
}

uint64_t Board::hash() const {
    // Simple hash for now, will improve with Zobrist hashing (Week 6)
    return player ^ (opponent << 1);
}

uint64_t Board::calc_legal_impl() const {
    // TODO: Implement legal move calculation using bitboard shifts (Week 1-2)
    // This is the core bitboard algorithm to be learned
    return 0ULL;
}

uint64_t Board::calc_flip(int pos) const {
    // TODO: Implement flip calculation (Week 2)
    return 0ULL;
}

} // namespace core
} // namespace reversi

