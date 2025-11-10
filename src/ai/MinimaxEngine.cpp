/*
 * MinimaxEngine.cpp - Implementation of Minimax search
 * COMP390 Honours Year Project
 */

#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace reversi::ai {

// Infinity constant (avoid overflow in negation)
constexpr int INF = std::numeric_limits<int>::max() / 2;

// Constructor implementations
MinimaxEngine::MinimaxEngine() : config_(), tt_(config_.tt_size_bits) {}

MinimaxEngine::MinimaxEngine(const Config& config) 
    : config_(config), tt_(config.tt_size_bits) {}

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
    
    // Reset TT statistics for this search (if enabled)
    if (config_.use_transposition) {
        tt_.reset_stats();
    }
    
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
    
    // Query transposition table (if enabled)
    if (config_.use_transposition) {
        uint64_t hash = board.hash();
        TTEntry* entry = tt_.probe(hash);
        
        if (entry && entry->depth >= depth) {
            // Found cached entry with sufficient depth
            int score = entry->score;
            
            // Adjust score for current depth (if needed)
            // Note: In practice, we store scores relative to current position
            
            // Use cached result based on entry type
            // TTFlag values: EXACT=0, LOWER_BOUND=1, UPPER_BOUND=2
            if (entry->flag == 0) {  // EXACT
                return score;
            } else if (entry->flag == 1) {  // LOWER_BOUND
                // Beta cutoff: score >= beta
                if (score >= beta) {
                    return score;
                }
            } else if (entry->flag == 2) {  // UPPER_BOUND
                // Alpha cutoff: score <= alpha
                if (score <= alpha) {
                    return score;
                }
            }
        }
    }
    
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
    
    // Move ordering: prioritize transposition table best move
    std::vector<int> ordered_moves = moves;
    if (config_.use_transposition) {
        uint64_t hash = board.hash();
        TTEntry* entry = tt_.probe(hash);
        if (entry && entry->best_move >= 0) {
            // Move TT best move to front
            auto it = std::find(ordered_moves.begin(), ordered_moves.end(), entry->best_move);
            if (it != ordered_moves.end()) {
                std::swap(ordered_moves[0], *it);
            }
        }
    }
    
    // Negamax recursion
    int best_score = -INF;
    int best_move = ordered_moves[0];
    int original_alpha = alpha;
    
    for (int move : ordered_moves) {
        // Make move
        reversi::core::Board next = board;
        next.make_move(move);
        
        // Recursive search (opponent's turn, negate score)
        int score = -negamax(next, depth - 1, -beta, -alpha);
        
        // Update best score and move
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
        
        // Alpha-Beta pruning
        if (config_.use_alpha_beta) {
            alpha = std::max(alpha, score);
            if (alpha >= beta) {
                // Beta cutoff: opponent won't allow this position
                break;
            }
        }
    }
    
    // Store result in transposition table (if enabled)
    if (config_.use_transposition) {
        uint64_t hash = board.hash();
        TTEntry new_entry;
        new_entry.hash = hash;
        new_entry.score = best_score;
        new_entry.depth = depth;
        new_entry.best_move = best_move;
        
        // Determine entry type based on alpha-beta bounds
        // TTFlag values: EXACT=0, LOWER_BOUND=1, UPPER_BOUND=2
        if (best_score <= original_alpha) {
            new_entry.flag = 2;  // UPPER_BOUND
        } else if (best_score >= beta) {
            new_entry.flag = 1;  // LOWER_BOUND
        } else {
            new_entry.flag = 0;  // EXACT
        }
        
        tt_.store(new_entry);
    }
    
    return best_score;
}

} // namespace reversi::ai

