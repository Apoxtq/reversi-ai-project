/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Move Implementation
 */

#include "Move.hpp"
#include <sstream>

namespace reversi {
namespace core {

std::string Move::to_string() const {
    if (is_pass()) {
        return "PASS";
    }
    if (is_null()) {
        return "NULL";
    }
    
    int row = position / 8;
    int col = position % 8;
    
    char col_char = 'a' + col;
    char row_char = '8' - row;
    
    std::ostringstream oss;
    oss << col_char << row_char;
    return oss.str();
}

Move Move::from_string(const std::string& str) {
    if (str == "PASS" || str == "pass") {
        return Move(PASS);
    }
    
    if (str.length() != 2) {
        return Move(NULL_MOVE);
    }
    
    char col_char = str[0];
    char row_char = str[1];
    
    // Convert to lowercase if needed
    if (col_char >= 'A' && col_char <= 'H') {
        col_char = col_char - 'A' + 'a';
    }
    
    if (col_char < 'a' || col_char > 'h' || row_char < '1' || row_char > '8') {
        return Move(NULL_MOVE);
    }
    
    int col = col_char - 'a';
    int row = '8' - row_char;
    
    return Move(row * 8 + col);
}

} // namespace core
} // namespace reversi

