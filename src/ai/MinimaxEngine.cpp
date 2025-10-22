/*
 * MinimaxEngine.cpp - Implementation of Minimax search
 * COMP390 Honours Year Project
 */

#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <iomanip>

namespace reversi::ai {

// Infinity constant (avoid overflow in negation)
constexpr int INF = std::numeric_limits<int>::max() / 2;

// Constructor implementations
MinimaxEngine::MinimaxEngine() : config_() {}

MinimaxEngine::MinimaxEngine(const Config& config) : config_(config) {}

void MinimaxEngine::SearchResult::print() const {
    std::cout << "Search Result:\n";
    std::cout << "  Best move: " << best_move << "\n";
    std::cout << "  Score: " << score << "\n";
    std::cout << "  Nodes searched: " << nodes_searched << "\n";
    std::cout << "  Depth: " << depth_reached << "\n";
    std::cout << "  Time: " << std::fixed << std::setprecision(2) << time_ms << " ms\n";
    std::cout << "  Speed: " << std::fixed << std::setprecision(2) 
              << nodes_per_sec() / 1e6 << " M nodes/sec\n";
}

MinimaxEngine::SearchResult MinimaxEngine::find_best_move(
    const reversi::core::Board& board) 
{
    using Clock = std::chrono::high_resolution_clock;
    auto start = Clock::now();
    nodes_searched_ = 0;
    
    const auto moves = board.get_legal_moves();
    
    // Special case: no legal moves (should pass)
    if (moves.empty()) {
        return {-1, 0, 0, 0, 0.0};
    }
    
    // Special case: only one legal move (no search needed)
    if (moves.size() == 1) {
        auto end = Clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
        return {moves[0], 0, 1, config_.max_depth, time_ms};
    }
    
    // Initialize search parameters
    int best_move = moves[0];
    int best_score = -INF;
    int alpha = -INF;
    int beta = INF;
    
    // Root level: search all legal moves
    for (int move : moves) {
        // Make move
        reversi::core::Board next = board;
        next.make_move(move);
        
        // Search opponent's response (negate score)
        int score = -negamax(next, config_.max_depth - 1, -beta, -alpha);
        
        // Update best move
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
        
        // Update alpha for alpha-beta pruning
        if (config_.use_alpha_beta) {
            alpha = std::max(alpha, score);
            // No pruning at root level (we want to explore all moves)
        }
    }
    
    // Calculate elapsed time
    auto end = Clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
    
    return {
        best_move,
        best_score,
        nodes_searched_,
        config_.max_depth,
        time_ms
    };
}

int MinimaxEngine::negamax(
    reversi::core::Board& board, 
    int depth, 
    int alpha, 
    int beta) 
{
    ++nodes_searched_;
    
    // Leaf node: evaluate position
    if (depth == 0) {
        return Evaluator::evaluate(board);
    }
    
    // Terminal node: game over
    if (board.is_terminal()) {
        const int diff = board.count_player() - board.count_opponent();
        // Large score to indicate definitive outcome
        // Multiply by (depth + 1) to prefer faster wins
        return diff * 10000 * (depth + 1);
    }
    
    // Get legal moves for current player
    const auto moves = board.get_legal_moves();
    
    // No legal moves: must pass
    if (moves.empty()) {
        reversi::core::Board next = board;
        next.pass();
        // Opponent's turn, negate score and swap alpha-beta
        return -negamax(next, depth - 1, -beta, -alpha);
    }
    
    // Negamax recursion
    int best_score = -INF;
    
    for (int move : moves) {
        // Make move
        reversi::core::Board next = board;
        next.make_move(move);
        
        // Recursive search (opponent's turn, negate score)
        int score = -negamax(next, depth - 1, -beta, -alpha);
        
        // Update best score
        best_score = std::max(best_score, score);
        
        // Alpha-Beta pruning
        if (config_.use_alpha_beta) {
            alpha = std::max(alpha, score);
            if (alpha >= beta) {
                // Beta cutoff: opponent won't allow this position
                break;
            }
        }
    }
    
    return best_score;
}

} // namespace reversi::ai

