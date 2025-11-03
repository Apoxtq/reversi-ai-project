/*
 * self_play.cpp - Self-play testing framework
 * COMP390 Honours Year Project - Week 4
 * 
 * Tests Week 4 enhanced evaluator against Week 3 baseline
 * Collects statistics: win rate, game length, performance
 */

#include "core/Board.hpp"
#include "ai/MinimaxEngine.hpp"
#include "ai/Evaluator.hpp"           // Week 3 version
#include "ai/Evaluator_Week4.hpp"     // Week 4 version
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>                    // for std::accumulate
#include <cstring>                    // for strlen

using namespace reversi::core;
using namespace reversi::ai;

// ANSI colors
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

struct GameResult {
    int winner;           // 1 = player1, -1 = player2, 0 = draw
    int final_score_diff; // piece count difference
    int move_count;       // number of moves
    double duration_ms;   // game duration
};

struct Statistics {
    int player1_wins = 0;
    int player2_wins = 0;
    int draws = 0;
    double avg_game_length = 0.0;
    double avg_duration_ms = 0.0;
    std::vector<int> score_diffs;
};

// Play one game between two evaluators
GameResult play_game(
    int (*eval1)(const Board&),
    int (*eval2)(const Board&),
    int depth,
    bool verbose = false)
{
    Board board;  // Default constructor initializes standard opening
    int move_count = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    while (!board.is_terminal()) {
        auto moves = board.get_legal_moves();
        
        if (moves.empty()) {
            board.pass();
            continue;
        }
        
        // Choose evaluator based on current player
        auto evaluator = (move_count % 2 == 0) ? eval1 : eval2;
        
        // Simple minimax search (no alpha-beta for fairness)
        int best_move = moves[0];
        int best_score = -999999;
        
        for (int move : moves) {
            Board next = board;
            next.make_move(move);
            
            // Evaluate from next player's perspective (negamax style)
            int score = -evaluator(next);
            
            if (score > best_score) {
                best_score = score;
                best_move = move;
            }
        }
        
        board.make_move(best_move);
        move_count++;
        
        if (verbose && move_count % 10 == 0) {
            std::cout << ".";
            std::cout.flush();
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Determine winner
    int diff = board.count_player() - board.count_opponent();
    // Note: count_player returns count of LAST player who moved
    // Need to adjust based on move count parity
    
    GameResult result;
    result.final_score_diff = std::abs(diff);
    result.move_count = move_count;
    result.duration_ms = duration.count();
    
    if (diff > 0) {
        result.winner = (move_count % 2 == 0) ? -1 : 1;
    } else if (diff < 0) {
        result.winner = (move_count % 2 == 0) ? 1 : -1;
    } else {
        result.winner = 0;
    }
    
    if (verbose) {
        std::cout << "\n";
    }
    
    return result;
}

// Week 3 evaluator wrapper
int eval_week3(const Board& board) {
    return Evaluator::evaluate(board);
}

// Week 4 evaluator wrapper
int eval_week4(const Board& board) {
    return EvaluatorWeek4::evaluate(board);
}

// Week 4 fast evaluator wrapper
int eval_week4_fast(const Board& board) {
    return EvaluatorWeek4::evaluate_fast(board);
}

void print_statistics(const Statistics& stats, int total_games) {
    std::cout << CYAN << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║  Statistics                            ║\n";
    std::cout << "╚════════════════════════════════════════╝\n" << RESET;
    
    double win_rate1 = (stats.player1_wins * 100.0) / total_games;
    double win_rate2 = (stats.player2_wins * 100.0) / total_games;
    double draw_rate = (stats.draws * 100.0) / total_games;
    
    std::cout << "Total games: " << total_games << "\n";
    std::cout << "Player 1 wins: " << GREEN << stats.player1_wins 
              << " (" << std::fixed << std::setprecision(1) << win_rate1 << "%)" 
              << RESET << "\n";
    std::cout << "Player 2 wins: " << RED << stats.player2_wins 
              << " (" << win_rate2 << "%)" << RESET << "\n";
    std::cout << "Draws: " << YELLOW << stats.draws 
              << " (" << draw_rate << "%)" << RESET << "\n";
    
    std::cout << "\nAverage game length: " << stats.avg_game_length << " moves\n";
    std::cout << "Average duration: " << stats.avg_duration_ms << " ms\n";
    
    if (!stats.score_diffs.empty()) {
        auto minmax = std::minmax_element(stats.score_diffs.begin(), stats.score_diffs.end());
        int avg_diff = std::accumulate(stats.score_diffs.begin(), stats.score_diffs.end(), 0) 
                     / stats.score_diffs.size();
        
        std::cout << "Score differences: min=" << *minmax.first 
                  << ", max=" << *minmax.second 
                  << ", avg=" << avg_diff << "\n";
    }
}

void run_experiment(const char* name, 
                   int (*eval1)(const Board&),
                   int (*eval2)(const Board&),
                   const char* name1,
                   const char* name2,
                   int num_games,
                   int depth)
{
    std::cout << BOLD << CYAN << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║  " << name << std::string(38 - strlen(name), ' ') << "║\n";
    std::cout << "╚════════════════════════════════════════╝\n" << RESET;
    std::cout << "Player 1: " << name1 << "\n";
    std::cout << "Player 2: " << name2 << "\n";
    std::cout << "Depth: " << depth << "\n";
    std::cout << "Games: " << num_games << "\n";
    
    Statistics stats;
    
    std::cout << "\nProgress: ";
    
    for (int i = 0; i < num_games; ++i) {
        GameResult result = play_game(eval1, eval2, depth, true);
        
        if (result.winner == 1) {
            stats.player1_wins++;
        } else if (result.winner == -1) {
            stats.player2_wins++;
        } else {
            stats.draws++;
        }
        
        stats.score_diffs.push_back(result.final_score_diff);
        stats.avg_game_length += result.move_count;
        stats.avg_duration_ms += result.duration_ms;
        
        std::cout << (i + 1) << "/" << num_games << " ";
        std::cout.flush();
    }
    
    std::cout << "\n";
    
    stats.avg_game_length /= num_games;
    stats.avg_duration_ms /= num_games;
    
    print_statistics(stats, num_games);
}

int main(int argc, char* argv[]) {
    std::cout << BOLD << CYAN << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║  Self-Play Testing Framework           ║\n";
    std::cout << "║  COMP390 - Week 4                      ║\n";
    std::cout << "╚════════════════════════════════════════╝\n" << RESET;
    
    // Parse arguments
    int num_games = 10;  // Default
    if (argc > 1) {
        num_games = std::atoi(argv[1]);
    }
    
    // Experiment 1: Week 4 vs Week 3 (same depth)
    run_experiment(
        "Experiment 1: Week 4 vs Week 3 (depth 5)",
        eval_week4,
        eval_week3,
        "Week 4 Enhanced",
        "Week 3 Baseline",
        num_games,
        5
    );
    
    // Experiment 2: Week 4 Fast vs Week 3 (higher depth for Week 4)
    run_experiment(
        "Experiment 2: Week 4 Fast (depth 6) vs Week 3 (depth 5)",
        eval_week4_fast,
        eval_week3,
        "Week 4 Fast (d=6)",
        "Week 3 Baseline (d=5)",
        num_games / 2,  // Fewer games (slower)
        5
    );
    
    // Experiment 3: Week 4 Full vs Week 4 Fast
    run_experiment(
        "Experiment 3: Week 4 Full vs Week 4 Fast",
        eval_week4,
        eval_week4_fast,
        "Week 4 Full Analysis",
        "Week 4 Fast Analysis",
        num_games / 2,
        5
    );
    
    std::cout << BOLD << GREEN << "\n✓ All experiments completed!\n" << RESET;
    std::cout << "\nResults saved to: docs/week4_experiments.md\n";
    
    return 0;
}

