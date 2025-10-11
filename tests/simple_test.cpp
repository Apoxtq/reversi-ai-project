/*
 * Simple test program for legal move generation
 * COMP390 Honours Year Project
 */

#include "core/Board.hpp"
#include <iostream>
#include <vector>

using namespace reversi::core;

std::string pos_to_algebraic(int pos) {
    int row = pos / 8;
    int col = pos % 8;
    char col_char = 'a' + col;
    char row_char = '8' - row;
    return std::string() + col_char + row_char;
}

int main() {
    std::cout << "=================================================\n";
    std::cout << "Reversi Legal Move Generation Test\n";
    std::cout << "=================================================\n\n";
    
    Board board;
    
    std::cout << "Initial board:\n";
    board.print();
    std::cout << "\n";
    
    std::cout << "Player (White O) pieces: " << board.count_player() << "\n";
    std::cout << "Opponent (Black X) pieces: " << board.count_opponent() << "\n\n";
    
    // Get legal moves
    auto legal = board.get_legal_moves();
    std::cout << "Number of legal moves: " << legal.size() << "\n";
    
    if (legal.size() > 0) {
        std::cout << "Legal moves: ";
        for (int pos : legal) {
            std::cout << pos_to_algebraic(pos) << "(" << pos << ") ";
        }
        std::cout << "\n\n";
    }
    
    std::cout << "Expected: 4 legal moves (d3, c4, f5, e6)\n";
    std::cout << "Result: " << (legal.size() == 4 ? "PASS" : "FAIL") << "\n\n";
    
    // Test make_move
    std::cout << "=================================================\n";
    std::cout << "Testing move execution\n";
    std::cout << "=================================================\n\n";
    
    if (legal.size() > 0) {
        int move_pos = legal[0];
        std::cout << "Playing move at " << pos_to_algebraic(move_pos) << " (pos " << move_pos << ")\n\n";
        
        uint64_t flips = board.calc_flip(move_pos);
        std::cout << "Pieces to flip: " << std::popcount(flips) << "\n";
        
        board.make_move(move_pos);
        
        std::cout << "\nBoard after move:\n";
        board.print();
        std::cout << "\n";
        
        std::cout << "New player pieces: " << board.count_player() << "\n";
        std::cout << "New opponent pieces: " << board.count_opponent() << "\n\n";
        
        auto new_legal = board.get_legal_moves();
        std::cout << "Opponent's legal moves: " << new_legal.size() << "\n";
        if (new_legal.size() > 0) {
            std::cout << "Moves: ";
            for (int pos : new_legal) {
                std::cout << pos_to_algebraic(pos) << " ";
            }
            std::cout << "\n";
        }
    }
    
    std::cout << "\n=================================================\n";
    std::cout << "Test completed!\n";
    std::cout << "=================================================\n";
    
    return 0;
}

