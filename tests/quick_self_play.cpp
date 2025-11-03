/*
 * Quick self-play test - simplified version
 * COMP390 Week 4
 */

#include "core/Board.hpp"
#include "ai/Evaluator.hpp"
#include "ai/Evaluator_Week4.hpp"
#include <iostream>
#include <chrono>

using namespace reversi::core;
using namespace reversi::ai;

// Simple greedy play - pick move with best immediate eval
int greedy_play(Board& board, int (*evaluator)(const Board&)) {
    auto moves = board.get_legal_moves();
    if (moves.empty()) {
        board.pass();
        return -1;
    }
    
    int best_move = moves[0];
    int best_score = -999999;
    
    for (int move : moves) {
        Board next = board;
        next.make_move(move);
        int score = -evaluator(next);  // Negamax style
        
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }
    
    board.make_move(best_move);
    return best_move;
}

int play_one_game(int (*eval1)(const Board&), int (*eval2)(const Board&)) {
    Board board;
    int moves = 0;
    
    while (!board.is_terminal() && moves < 100) {
        int move = greedy_play(board, (moves % 2 == 0) ? eval1 : eval2);
        if (move >= 0) moves++;
    }
    
    int diff = board.count_player() - board.count_opponent();
    
    if (diff > 0) return (moves % 2 == 0) ? -1 : 1;
    else if (diff < 0) return (moves % 2 == 0) ? 1 : -1;
    else return 0;
}

int eval_week3(const Board& b) { return Evaluator::evaluate(b); }
int eval_week4(const Board& b) { return EvaluatorWeek4::evaluate(b); }

int main() {
    std::cout << "\n=== Quick Self-Play: Week 4 vs Week 3 ===\n\n";
    
    int week4_wins = 0, week3_wins = 0, draws = 0;
    int num_games = 10;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_games; ++i) {
        std::cout << "Game " << (i+1) << "... ";
        std::cout.flush();
        
        int result = play_one_game(eval_week4, eval_week3);
        
        if (result == 1) {
            week4_wins++;
            std::cout << "Week 4 wins\n";
        } else if (result == -1) {
            week3_wins++;
            std::cout << "Week 3 wins\n";
        } else {
            draws++;
            std::cout << "Draw\n";
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\n=== Results ===\n";
    std::cout << "Week 4 wins: " << week4_wins << " (" << (week4_wins*100.0/num_games) << "%)\n";
    std::cout << "Week 3 wins: " << week3_wins << " (" << (week3_wins*100.0/num_games) << "%)\n";
    std::cout << "Draws: " << draws << "\n";
    std::cout << "Total time: " << duration.count() << " ms\n";
    std::cout << "Avg per game: " << (duration.count() / num_games) << " ms\n";
    
    return 0;
}

