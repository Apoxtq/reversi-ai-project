/*
 * AI Demo - Interactive Minimax vs Human Game
 * COMP390 Honours Year Project
 * 
 * Author: Tianqixing
 */

#include "core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include "ai/Evaluator.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>

using namespace reversi::core;
using namespace reversi::ai;

void print_board_with_moves(const Board& board) {
    auto moves = board.get_legal_moves();
    uint64_t moves_bb = board.legal_moves();
    
    std::cout << "\n   A B C D E F G H\n";
    std::cout << "  +----------------+\n";
    
    for (int row = 0; row < 8; ++row) {
        std::cout << (row + 1) << " |";
        for (int col = 0; col < 8; ++col) {
            int pos = row * 8 + col;
            uint64_t mask = 1ULL << pos;
            
            if (board.player & mask) {
                std::cout << " X";  // Current player
            } else if (board.opponent & mask) {
                std::cout << " O";  // Opponent
            } else if (moves_bb & mask) {
                std::cout << " ·";  // Legal move
            } else {
                std::cout << "  ";
            }
        }
        std::cout << " | " << (row + 1) << "\n";
    }
    
    std::cout << "  +----------------+\n";
    std::cout << "   A B C D E F G H\n";
}

void print_game_info(const Board& board, const std::string& current_player) {
    int player_count = board.count_player();
    int opponent_count = board.count_opponent();
    int total = player_count + opponent_count;
    int empty = 64 - total;
    
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "Current Player: " << current_player << "\n";
    std::cout << "Score - X: " << player_count << " | O: " << opponent_count;
    std::cout << " | Empty: " << empty << "\n";
    std::cout << "Evaluation: " << Evaluator::evaluate(board) << "\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
}

int parse_move(const std::string& input) {
    if (input.length() != 2) return -1;
    
    char col_char = std::toupper(input[0]);
    char row_char = input[1];
    
    if (col_char < 'A' || col_char > 'H') return -1;
    if (row_char < '1' || row_char > '8') return -1;
    
    int col = col_char - 'A';
    int row = row_char - '1';
    
    return row * 8 + col;
}

void play_human_vs_ai(int ai_depth, bool human_first) {
    Board board;
    MinimaxEngine engine(MinimaxEngine::Config{ai_depth, true});
    
    std::string human_piece = human_first ? "X" : "O";
    std::string ai_piece = human_first ? "O" : "X";
    
    // If AI plays first, make it go to opponent perspective
    if (!human_first) {
        board.pass();  // Swap perspective so AI is "player"
    }
    
    bool human_turn = human_first;
    int consecutive_passes = 0;
    int move_number = 1;
    
    std::cout << "\n╔═══════════════════════════════════════════╗\n";
    std::cout << "║  Reversi AI - Human vs Minimax Engine    ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n";
    std::cout << "\nYou are playing as: " << human_piece << "\n";
    std::cout << "AI is playing as: " << ai_piece << "\n";
    std::cout << "AI Depth: " << ai_depth << " plies\n";
    
    while (true) {
        std::string current_player = human_turn ? human_piece : ai_piece;
        print_game_info(board, current_player);
        print_board_with_moves(board);
        
        auto legal_moves = board.get_legal_moves();
        
        // Check if current player has no legal moves
        if (legal_moves.empty()) {
            std::cout << "\n⚠ " << current_player << " has no legal moves! Passing turn...\n";
            board.pass();
            human_turn = !human_turn;
            consecutive_passes++;
            
            if (consecutive_passes >= 2) {
                break;  // Game over
            }
            
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        consecutive_passes = 0;  // Reset counter
        
        // Show legal moves
        std::cout << "\nLegal moves: ";
        for (int move : legal_moves) {
            int row = move / 8;
            int col = move % 8;
            std::cout << char('A' + col) << (row + 1) << " ";
        }
        std::cout << "\n";
        
        int chosen_move = -1;
        
        if (human_turn) {
            // Human's turn
            while (true) {
                std::cout << "\nYour move (e.g., D3) or 'q' to quit: ";
                std::string input;
                std::cin >> input;
                
                if (input == "q" || input == "Q") {
                    std::cout << "Game abandoned.\n";
                    return;
                }
                
                chosen_move = parse_move(input);
                
                // Check if move is legal
                bool is_legal = false;
                for (int m : legal_moves) {
                    if (m == chosen_move) {
                        is_legal = true;
                        break;
                    }
                }
                
                if (is_legal) {
                    break;
                } else {
                    std::cout << "❌ Illegal move! Try again.\n";
                }
            }
        } else {
            // AI's turn
            std::cout << "\n🤖 AI is thinking";
            std::cout.flush();
            
            auto start = std::chrono::high_resolution_clock::now();
            auto result = engine.find_best_move(board);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            chosen_move = result.best_move;
            
            int row = chosen_move / 8;
            int col = chosen_move % 8;
            
            std::cout << "\n\n🤖 AI plays: " << char('A' + col) << (row + 1) << "\n";
            std::cout << "   Score: " << result.score;
            std::cout << " | Nodes: " << result.nodes_searched;
            std::cout << " | Time: " << duration.count() << "ms";
            std::cout << " | Speed: " << std::fixed << std::setprecision(2) 
                      << (result.nodes_per_sec() / 1e6) << "M/s\n";
        }
        
        // Make the move
        board.make_move(chosen_move);
        human_turn = !human_turn;
        move_number++;
    }
    
    // Game over
    print_game_info(board, "Game Over");
    print_board_with_moves(board);
    
    int player_final = board.count_player();
    int opponent_final = board.count_opponent();
    
    // Determine winner (remember perspective might be swapped)
    int human_score = human_first ? player_final : opponent_final;
    int ai_score = human_first ? opponent_final : player_final;
    
    std::cout << "\n╔═══════════════════════════════════════════╗\n";
    std::cout << "║              GAME OVER                    ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n";
    std::cout << "\nFinal Score:\n";
    std::cout << "  Human (" << human_piece << "): " << human_score << "\n";
    std::cout << "  AI    (" << ai_piece << "): " << ai_score << "\n";
    
    if (human_score > ai_score) {
        std::cout << "\n🎉 Congratulations! You WIN!\n";
    } else if (ai_score > human_score) {
        std::cout << "\n🤖 AI WINS! Better luck next time!\n";
    } else {
        std::cout << "\n🤝 It's a DRAW!\n";
    }
}

void ai_vs_ai_demo(int depth1, int depth2, bool fast_mode = false) {
    Board board;
    MinimaxEngine engine1(MinimaxEngine::Config{depth1, true});
    MinimaxEngine engine2(MinimaxEngine::Config{depth2, true});
    
    std::cout << "\n╔═══════════════════════════════════════════╗\n";
    std::cout << "║       AI vs AI Demonstration              ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n";
    std::cout << "\nAI #1 (X) - Depth: " << depth1 << "\n";
    std::cout << "AI #2 (O) - Depth: " << depth2 << "\n\n";
    
    bool ai1_turn = true;
    int consecutive_passes = 0;
    int move_number = 1;
    
    while (true) {
        if (!fast_mode) {
            std::string current = ai1_turn ? "AI #1 (X)" : "AI #2 (O)";
            print_game_info(board, current);
            print_board_with_moves(board);
        }
        
        auto legal_moves = board.get_legal_moves();
        
        if (legal_moves.empty()) {
            std::cout << "\nPass!\n";
            board.pass();
            ai1_turn = !ai1_turn;
            consecutive_passes++;
            
            if (consecutive_passes >= 2) break;
            
            if (!fast_mode) {
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            continue;
        }
        
        consecutive_passes = 0;
        
        auto& engine = ai1_turn ? engine1 : engine2;
        auto result = engine.find_best_move(board);
        
        int row = result.best_move / 8;
        int col = result.best_move % 8;
        
        std::cout << "Move " << move_number << ": ";
        std::cout << (ai1_turn ? "AI#1" : "AI#2") << " plays ";
        std::cout << char('A' + col) << (row + 1);
        std::cout << " (score: " << result.score;
        std::cout << ", nodes: " << result.nodes_searched << ")\n";
        
        board.make_move(result.best_move);
        ai1_turn = !ai1_turn;
        move_number++;
        
        if (!fast_mode && move_number > 1) {
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    // Final result
    print_board_with_moves(board);
    int ai1_score = board.count_player();
    int ai2_score = board.count_opponent();
    
    std::cout << "\n╔═══════════════════════════════════════════╗\n";
    std::cout << "║              GAME OVER                    ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n";
    std::cout << "\nFinal Score:\n";
    std::cout << "  AI #1 (Depth " << depth1 << "): " << ai1_score << "\n";
    std::cout << "  AI #2 (Depth " << depth2 << "): " << ai2_score << "\n";
    
    if (ai1_score > ai2_score) {
        std::cout << "\n🏆 AI #1 WINS!\n";
    } else if (ai2_score > ai1_score) {
        std::cout << "\n🏆 AI #2 WINS!\n";
    } else {
        std::cout << "\n🤝 DRAW!\n";
    }
}

void show_menu() {
    std::cout << "\n╔═══════════════════════════════════════════╗\n";
    std::cout << "║  Reversi AI - Minimax Engine Demo        ║\n";
    std::cout << "║  COMP390 Honours Year Project             ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n";
    std::cout << "\nSelect mode:\n";
    std::cout << "  1. Human vs AI (You play X, go first)\n";
    std::cout << "  2. Human vs AI (You play O, go second)\n";
    std::cout << "  3. AI vs AI Demo\n";
    std::cout << "  4. Quick AI vs AI Benchmark\n";
    std::cout << "  5. Exit\n";
    std::cout << "\nYour choice: ";
}

int main() {
    while (true) {
        show_menu();
        
        int choice;
        std::cin >> choice;
        
        if (choice == 5) {
            std::cout << "\nThank you for using Reversi AI!\n";
            break;
        }
        
        if (choice == 1 || choice == 2) {
            std::cout << "\nSelect AI difficulty (depth):\n";
            std::cout << "  1. Easy (depth 4)\n";
            std::cout << "  2. Medium (depth 6)\n";
            std::cout << "  3. Hard (depth 8)\n";
            std::cout << "  4. Expert (depth 10)\n";
            std::cout << "Choice: ";
            
            int diff;
            std::cin >> diff;
            
            int depth = 4 + (diff - 1) * 2;
            if (depth < 4) depth = 4;
            if (depth > 10) depth = 10;
            
            bool human_first = (choice == 1);
            play_human_vs_ai(depth, human_first);
            
        } else if (choice == 3) {
            std::cout << "\nAI #1 depth: ";
            int d1;
            std::cin >> d1;
            std::cout << "AI #2 depth: ";
            int d2;
            std::cin >> d2;
            
            ai_vs_ai_demo(d1, d2, false);
            
        } else if (choice == 4) {
            ai_vs_ai_demo(6, 6, true);
            
        } else {
            std::cout << "\n❌ Invalid choice!\n";
        }
    }
    
    return 0;
}

