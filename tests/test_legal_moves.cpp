/*
 * Test program for legal move generation
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Tests the legal move generation implementation
 */

#include "../src/core/Board.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace reversi::core;

// Convert position to algebraic notation
std::string pos_to_algebraic(int pos) {
    int row = pos / 8;
    int col = pos % 8;
    char col_char = 'a' + col;
    char row_char = '8' - row;
    return std::string() + col_char + row_char;
}

// Print bitboard visually
void print_bitboard(uint64_t bb, const std::string& title) {
    std::cout << title << ":\n";
    std::cout << "  A B C D E F G H\n";
    for (int row = 0; row < 8; ++row) {
        std::cout << (8 - row) << " ";
        for (int col = 0; col < 8; ++col) {
            int pos = row * 8 + col;
            if (bb & (1ULL << pos)) {
                std::cout << "X ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << (8 - row) << "\n";
    }
    std::cout << "  A B C D E F G H\n\n";
}

void test_initial_position() {
    std::cout << "=== Test 1: Initial Position Legal Moves ===\n\n";
    
    Board board;
    
    std::cout << "Initial board:\n";
    board.print();
    std::cout << "\n";
    
    std::cout << "Player pieces: " << board.count_player() << "\n";
    std::cout << "Opponent pieces: " << board.count_opponent() << "\n\n";
    
    // Get legal moves
    auto legal = board.get_legal_moves();
    std::cout << "Number of legal moves: " << legal.size() << "\n";
    
    if (legal.size() > 0) {
        std::cout << "Legal moves: ";
        for (int pos : legal) {
            std::cout << pos_to_algebraic(pos) << " ";
        }
        std::cout << "\n\n";
    }
    
    // Visualize legal moves
    uint64_t legal_bb = board.legal_moves();
    print_bitboard(legal_bb, "Legal move positions");
    
    // Expected: 4 legal moves for Black (d3, c4, f5, e6)
    // Positions: d3=19, c4=26, f5=37, e6=44
    std::cout << "Expected legal moves for starting position:\n";
    std::cout << "Black (O) should have 4 legal moves: d3, c4, f5, e6\n";
    std::cout << "Actual count: " << legal.size() << "\n";
    
    if (legal.size() == 4) {
        std::cout << "[OK] PASS: Correct number of legal moves\n";
    } else {
        std::cout << "[FAIL] FAIL: Expected 4 moves\n";
    }
    
    std::cout << "\n" << std::string(50, '-') << "\n\n";
}

void test_make_move() {
    std::cout << "=== Test 2: Make Move and Update Board ===\n\n";
    
    Board board;
    
    std::cout << "Initial board:\n";
    board.print();
    std::cout << "\n";
    
    // Black plays d3 (position 19)
    int move_pos = 19; // d3
    std::cout << "Black plays " << pos_to_algebraic(move_pos) << "\n\n";
    
    // Show flips
    uint64_t flips = board.calc_flip(move_pos);
    std::cout << "Pieces to flip: " << std::popcount(flips) << "\n";
    print_bitboard(flips, "Flipped pieces");
    
    board.make_move(move_pos);
    
    std::cout << "Board after move:\n";
    board.print();
    std::cout << "\n";
    
    std::cout << "Player pieces (White, after swap): " << board.count_player() << "\n";
    std::cout << "Opponent pieces (Black): " << board.count_opponent() << "\n\n";
    
    // White's turn now
    auto legal = board.get_legal_moves();
    std::cout << "White's legal moves: " << legal.size() << "\n";
    if (legal.size() > 0) {
        std::cout << "Positions: ";
        for (int pos : legal) {
            std::cout << pos_to_algebraic(pos) << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n" << std::string(50, '-') << "\n\n";
}

void test_game_sequence() {
    std::cout << "=== Test 3: Play Several Moves ===\n\n";
    
    Board board;
    
    std::vector<int> moves = {19, 18, 26}; // d3, c3, c4
    std::vector<std::string> move_names = {"d3 (Black)", "c3 (White)", "c4 (Black)"};
    
    for (size_t i = 0; i < moves.size(); ++i) {
        std::cout << "Move " << (i + 1) << ": " << move_names[i] << "\n";
        board.make_move(moves[i]);
        board.print();
        std::cout << "Black: " << std::popcount(board.opponent) << " pieces\n";
        std::cout << "White: " << std::popcount(board.player) << " pieces\n";
        std::cout << "Legal moves: " << board.get_legal_moves().size() << "\n\n";
    }
    
    std::cout << std::string(50, '-') << "\n\n";
}

void test_terminal_state() {
    std::cout << "=== Test 4: Terminal State Detection ===\n\n";
    
    Board board;
    
    std::cout << "Initial position - is terminal? " << (board.is_terminal() ? "Yes" : "No") << "\n";
    
    if (!board.is_terminal()) {
        std::cout << "[OK] PASS: Game not over at start\n";
    } else {
        std::cout << "[FAIL] FAIL: Game should not be over\n";
    }
    
    // Create a near-terminal position (full board)
    board.player = 0xFFFFFFFFFFFFFF00ULL;
    board.opponent = 0x00000000000000FFULL;
    
    std::cout << "\nFull board position:\n";
    board.print();
    std::cout << "Is terminal? " << (board.is_terminal() ? "Yes" : "No") << "\n";
    
    std::cout << "\n" << std::string(50, '-') << "\n\n";
}

void test_bitboard_integrity() {
    std::cout << "=== Test 5: Bitboard Integrity ===\n\n";
    
    Board board;
    
    // Check initial position values
    std::cout << "Initial player bitboard:   0x" << std::hex << board.player << std::dec << "\n";
    std::cout << "Initial opponent bitboard: 0x" << std::hex << board.opponent << std::dec << "\n\n";
    
    // Verify no overlap
    if ((board.player & board.opponent) == 0) {
        std::cout << "[OK] PASS: No overlap between player and opponent\n";
    } else {
        std::cout << "[FAIL] FAIL: Player and opponent bitboards overlap!\n";
    }
    
    // Verify correct piece count
    int total = board.count_player() + board.count_opponent();
    std::cout << "Total pieces: " << total << " (expected 4)\n";
    
    if (total == 4) {
        std::cout << "[OK] PASS: Correct initial piece count\n";
    } else {
        std::cout << "[FAIL] FAIL: Wrong piece count\n";
    }
    
    std::cout << "\n" << std::string(50, '-') << "\n\n";
}

int main() {
    std::cout << "\n";
    std::cout << "+================================================+\n";
    std::cout << "|  Reversi Legal Move Generation Test Suite     |\n";
    std::cout << "|  COMP390 Honours Year Project (2025-26)       |\n";
    std::cout << "|  Student: Tianqixing (201821852)              |\n";
    std::cout << "+================================================+\n";
    std::cout << "\n";
    
    try {
        test_bitboard_integrity();
        test_initial_position();
        test_make_move();
        test_game_sequence();
        test_terminal_state();
        
        std::cout << "\n";
        std::cout << "+================================================+\n";
        std::cout << "|  All tests completed!                          |\n";
        std::cout << "+================================================+\n";
        std::cout << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

