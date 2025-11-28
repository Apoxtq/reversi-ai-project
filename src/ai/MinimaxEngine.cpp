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
MinimaxEngine::MinimaxEngine() : config_(), tt_(config_.tt_size_bits) {
    clear_killers();
    last_stats_.reset();
}

MinimaxEngine::MinimaxEngine(const Config& config) 
    : config_(config), tt_(config.tt_size_bits) {
    clear_killers();
    last_stats_.reset();
}

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
    search_start_ = Clock::now();
    nodes_searched_ = 0;
    time_exceeded_ = false;
    current_ply_ = 0;
    clear_killers();
    
    // Calculate time limit based on game phase
    if (config_.time_limit_ms > 0) {
        time_limit_ms_ = calculate_time_limit(board, config_.time_limit_ms);
    } else {
        time_limit_ms_ = 0;
    }
    
    // Reset TT statistics for this search (if enabled)
    if (config_.use_transposition) {
        tt_.reset_stats();
    }
    
    const auto moves = board.get_legal_moves();
    
    // Special case: no legal moves (should pass)
    if (moves.empty()) {
        auto end = Clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - search_start_).count();
        return {-1, 0, 0, 0, time_ms};
    }
    
    // Special case: only one legal move (no search needed)
    if (moves.size() == 1) {
        auto end = Clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - search_start_).count();
        return {moves[0], 0, 1, config_.max_depth, time_ms};
    }
    
    // Use iterative deepening if enabled
    if (config_.use_iterative_deepening) {
        return iterative_deepening_search(board);
    }
    
    // Standard fixed-depth search
    int best_move = moves[0];
    int best_score = -INF;
    int alpha = -INF;
    int beta = INF;
    
    // Root level: search all legal moves
    for (int move : moves) {
        if (time_exceeded()) break;
        
        // Make move
        reversi::core::Board next = board;
        next.make_move(move);
        
        // Search opponent's response
        int score;
        if (config_.use_pvs) {
            score = -pvs(next, config_.max_depth - 1, -beta, -alpha, false);
        } else {
            score = -negamax(next, config_.max_depth - 1, -beta, -alpha);
        }
        
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
    double time_ms = std::chrono::duration<double, std::milli>(end - search_start_).count();
    
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
    ++current_ply_;
    
    // Periodic time check
    if (time_limit_ms_ > 0 && nodes_searched_ % TIME_CHECK_INTERVAL == 0) {
        if (time_exceeded()) {
            --current_ply_;
            // Return a very negative score to indicate timeout
            // This will be pruned by alpha-beta, but won't mislead the search
            return -INF + 1;
        }
    }
    
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
                --current_ply_;
                return score;
            } else if (entry->flag == 1) {  // LOWER_BOUND
                // Beta cutoff: score >= beta
                if (score >= beta) {
                    --current_ply_;
                    return score;
                }
            } else if (entry->flag == 2) {  // UPPER_BOUND
                // Alpha cutoff: score <= alpha
                if (score <= alpha) {
                    --current_ply_;
                    return score;
                }
            }
        }
    }
    
    // Leaf node: evaluate position
    if (depth == 0) {
        int score = Evaluator::evaluate(board);
        --current_ply_;
        return score;
    }
    
    // Terminal node: game over
    if (board.is_terminal()) {
        const int diff = board.count_player() - board.count_opponent();
        // Large score to indicate definitive outcome
        // Multiply by (depth + 1) to prefer faster wins
        int score = diff * 10000 * (depth + 1);
        --current_ply_;
        return score;
    }
    
    // Get legal moves for current player
    const auto moves = board.get_legal_moves();
    
    // No legal moves: must pass
    if (moves.empty()) {
        reversi::core::Board next = board;
        next.pass();
        // Opponent's turn, negate score and swap alpha-beta
        int score = -negamax(next, depth - 1, -beta, -alpha);
        --current_ply_;
        return score;
    }
    
    // Move ordering: use comprehensive ordering if enabled
    std::vector<int> ordered_moves;
    if (config_.use_killer_moves || config_.use_transposition) {
        ordered_moves = order_moves(board, moves);
    } else {
        ordered_moves = moves;
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
                // Update killer move
                if (config_.use_killer_moves) {
                    update_killer(move, current_ply_);
                }
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
    
    --current_ply_;
    return best_score;
}

// Week 6: Iterative deepening search
MinimaxEngine::SearchResult MinimaxEngine::iterative_deepening_search(
    const reversi::core::Board& board) 
{
    using Clock = std::chrono::high_resolution_clock;
    
    SearchResult best_result;
    best_result.best_move = -1;
    best_result.score = -INF;
    best_result.depth_reached = 0;
    
    // Get predicted score from transposition table (if available)
    int predicted_score = 0;
    if (config_.use_transposition) {
        uint64_t hash = board.hash();
        TTEntry* entry = tt_.probe(hash);
        if (entry && entry->is_valid()) {
            predicted_score = entry->score;
        }
    }
    
    // Iterative deepening: search from depth 1 to max_depth
    for (int depth = 1; depth <= config_.max_depth; ++depth) {
        if (time_exceeded()) {
            // Use result from previous depth
            break;
        }
        
        SearchResult result;
        
        // Use aspiration windows if enabled and we have a predicted score
        if (config_.use_aspiration && depth > 1 && predicted_score != 0) {
            result = aspiration_search(board, depth, predicted_score);
        } else {
            // Standard search
            int alpha = -INF;
            int beta = INF;
            
            const auto moves = board.get_legal_moves();
            if (moves.empty()) {
                break;
            }
            
            int best_move = moves[0];
            int best_score = -INF;
            
            for (int move : moves) {
                if (time_exceeded()) break;
                
                reversi::core::Board next = board;
                next.make_move(move);
                
                int score;
                if (config_.use_pvs) {
                    score = -pvs(next, depth - 1, -beta, -alpha, false);
                } else {
                    score = -negamax(next, depth - 1, -beta, -alpha);
                }
                
                if (score > best_score) {
                    best_score = score;
                    best_move = move;
                }
                
                if (config_.use_alpha_beta) {
                    alpha = std::max(alpha, score);
                }
            }
            
            auto end = Clock::now();
            double time_ms = std::chrono::duration<double, std::milli>(end - search_start_).count();
            
            result.best_move = best_move;
            result.score = best_score;
            result.nodes_searched = nodes_searched_;
            result.depth_reached = depth;
            result.time_ms = time_ms;
        }
        
        // Update best result
        if (result.best_move >= 0) {
            best_result = result;
            predicted_score = result.score;  // Update prediction for next depth
        }
        
        // If we found a definitive win/loss, we can stop early
        if (std::abs(result.score) > 10000) {
            break;
        }
    }
    
    return best_result;
}

// AIStrategy interface implementation
reversi::core::Move MinimaxEngine::find_best_move(const reversi::core::Board& board, 
                                                   const SearchLimits& limits) {
    // Update config from limits
    Config search_config = config_;
    search_config.max_depth = limits.max_depth;
    search_config.time_limit_ms = limits.max_time_ms;
    
    // Enable iterative deepening if time limit is set
    if (limits.max_time_ms > 0) {
        search_config.use_iterative_deepening = true;
    }
    
    // Temporarily set config
    Config old_config = config_;
    config_ = search_config;
    
    // Perform search
    SearchResult result = find_best_move(board);
    
    // Restore config
    config_ = old_config;
    
    // Update statistics for AIStrategy interface
    last_stats_.nodes_searched = result.nodes_searched;
    last_stats_.depth_reached = result.depth_reached;
    last_stats_.time_elapsed_ms = static_cast<int>(result.time_ms);
    last_stats_.nodes_per_second = result.nodes_per_sec();
    
    // Convert to Move
    if (result.best_move == -1) {
        return reversi::core::Move(reversi::core::Move::PASS);
    } else if (result.best_move >= 0 && result.best_move < 64) {
        return reversi::core::Move(result.best_move);
    } else {
        return reversi::core::Move(reversi::core::Move::NULL_MOVE);
    }
}

// Week 6: Aspiration window search
MinimaxEngine::SearchResult MinimaxEngine::aspiration_search(
    const reversi::core::Board& board, 
    int depth, 
    int predicted_score) 
{
    using Clock = std::chrono::high_resolution_clock;
    
    const int window = config_.aspiration_window;
    int alpha = predicted_score - window;
    int beta = predicted_score + window;
    
    const auto moves = board.get_legal_moves();
    if (moves.empty()) {
        auto end = Clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - search_start_).count();
        return {-1, 0, nodes_searched_, depth, time_ms};
    }
    
    int best_move = moves[0];
    int best_score = -INF;
    
    // First search with aspiration window
    for (int move : moves) {
        if (time_exceeded()) break;
        
        reversi::core::Board next = board;
        next.make_move(move);
        
        int score;
        if (config_.use_pvs) {
            score = -pvs(next, depth - 1, -beta, -alpha, false);
        } else {
            score = -negamax(next, depth - 1, -beta, -alpha);
        }
        
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
        
        if (config_.use_alpha_beta) {
            alpha = std::max(alpha, score);
        }
    }
    
    // If aspiration window failed, re-search with full window
    if (best_score <= predicted_score - window) {
        // Failed low: re-search with full window below
        alpha = -INF;
        beta = predicted_score;
        best_score = -INF;
        
        for (int move : moves) {
            if (time_exceeded()) break;
            
            reversi::core::Board next = board;
            next.make_move(move);
            
            int score;
            if (config_.use_pvs) {
                score = -pvs(next, depth - 1, -beta, -alpha, false);
            } else {
                score = -negamax(next, depth - 1, -beta, -alpha);
            }
            
            if (score > best_score) {
                best_score = score;
                best_move = move;
            }
            
            if (config_.use_alpha_beta) {
                alpha = std::max(alpha, score);
            }
        }
    } else if (best_score >= predicted_score + window) {
        // Failed high: re-search with full window above
        alpha = predicted_score;
        beta = INF;
        best_score = -INF;
        
        for (int move : moves) {
            if (time_exceeded()) break;
            
            reversi::core::Board next = board;
            next.make_move(move);
            
            int score;
            if (config_.use_pvs) {
                score = -pvs(next, depth - 1, -beta, -alpha, false);
            } else {
                score = -negamax(next, depth - 1, -beta, -alpha);
            }
            
            if (score > best_score) {
                best_score = score;
                best_move = move;
            }
            
            if (config_.use_alpha_beta) {
                alpha = std::max(alpha, score);
            }
        }
    }
    
    auto end = Clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - search_start_).count();
    
    return {best_move, best_score, nodes_searched_, depth, time_ms};
}

// Week 6: Principal Variation Search (PVS) / NegaScout
int MinimaxEngine::pvs(
    reversi::core::Board& board, 
    int depth, 
    int alpha, 
    int beta, 
    bool is_pv) 
{
    ++nodes_searched_;
    ++current_ply_;
    
    // Periodic time check
    if (time_limit_ms_ > 0 && nodes_searched_ % TIME_CHECK_INTERVAL == 0) {
        if (time_exceeded()) {
            --current_ply_;
            // Return a very negative score to indicate timeout
            // This will be pruned by alpha-beta, but won't mislead the search
            return -INF + 1;
        }
    }
    
    // Query transposition table
    if (config_.use_transposition) {
        uint64_t hash = board.hash();
        TTEntry* entry = tt_.probe(hash);
        
        if (entry && entry->depth >= depth) {
            int score = entry->score;
            
            if (entry->flag == 0) {  // EXACT
                --current_ply_;
                return score;
            } else if (entry->flag == 1 && score >= beta) {  // LOWER_BOUND
                --current_ply_;
                return score;
            } else if (entry->flag == 2 && score <= alpha) {  // UPPER_BOUND
                --current_ply_;
                return score;
            }
        }
    }
    
    // Leaf node
    if (depth == 0) {
        int score = Evaluator::evaluate(board);
        --current_ply_;
        return score;
    }
    
    // Terminal node
    if (board.is_terminal()) {
        const int diff = board.count_player() - board.count_opponent();
        int score = diff * 10000 * (depth + 1);
        --current_ply_;
        return score;
    }
    
    // Get legal moves
    const auto moves = board.get_legal_moves();
    
    // No legal moves: must pass
    if (moves.empty()) {
        reversi::core::Board next = board;
        next.pass();
        int score = -pvs(next, depth - 1, -beta, -alpha, is_pv);
        --current_ply_;
        return score;
    }
    
    // Order moves
    std::vector<int> ordered_moves = order_moves(board, moves);
    
    int best_score = -INF;
    int best_move = ordered_moves[0];
    int original_alpha = alpha;
    
    // First move: full window search
    {
        reversi::core::Board next = board;
        next.make_move(ordered_moves[0]);
        best_score = -pvs(next, depth - 1, -beta, -alpha, true);
        best_move = ordered_moves[0];
        
        if (config_.use_alpha_beta) {
            alpha = std::max(alpha, best_score);
            if (alpha >= beta) {
                // Beta cutoff
                if (config_.use_killer_moves) {
                    update_killer(ordered_moves[0], current_ply_);
                }
                --current_ply_;
                return best_score;
            }
        }
    }
    
    // Subsequent moves: zero window search (null window)
    for (size_t i = 1; i < ordered_moves.size(); ++i) {
        if (time_exceeded()) break;
        
        reversi::core::Board next = board;
        next.make_move(ordered_moves[i]);
        
        // Zero window search: assume this move is not better
        // Search with window [alpha, alpha+1] to test if score > alpha
        int score = -pvs(next, depth - 1, -alpha - 1, -alpha, false);
        
        // If zero window fails (score > alpha), re-search with full window
        // Standard PVS: re-search if score > alpha (not just if in range)
        if (score > alpha) {
            if (score >= beta) {
                // Beta cutoff, no need to re-search
                if (config_.use_killer_moves) {
                    update_killer(ordered_moves[i], current_ply_);
                }
                best_score = score;
                best_move = ordered_moves[i];
                break;
            }
            // Re-search with full window [alpha, beta]
            score = -pvs(next, depth - 1, -beta, -alpha, true);
        }
        
        if (score > best_score) {
            best_score = score;
            best_move = ordered_moves[i];
        }
        
        if (config_.use_alpha_beta) {
            alpha = std::max(alpha, score);
            if (alpha >= beta) {
                // Beta cutoff
                if (config_.use_killer_moves) {
                    update_killer(ordered_moves[i], current_ply_);
                }
                break;
            }
        }
    }
    
    // Store in transposition table
    if (config_.use_transposition) {
        uint64_t hash = board.hash();
        TTEntry new_entry;
        new_entry.hash = hash;
        new_entry.score = best_score;
        new_entry.depth = depth;
        new_entry.best_move = best_move;
        
        if (best_score <= original_alpha) {
            new_entry.flag = 2;  // UPPER_BOUND
        } else if (best_score >= beta) {
            new_entry.flag = 1;  // LOWER_BOUND
        } else {
            new_entry.flag = 0;  // EXACT
        }
        
        tt_.store(new_entry);
    }
    
    --current_ply_;
    return best_score;
}

// Week 6: Time management
int MinimaxEngine::calculate_time_limit(
    const reversi::core::Board& board, 
    int total_time_ms) const 
{
    // Boundary check: ensure total_time_ms is positive
    if (total_time_ms <= 0) {
        return 0;
    }
    
    int player_count = board.count_player();
    int opponent_count = board.count_opponent();
    int empties = 64 - player_count - opponent_count;
    
    // Boundary check: ensure empties is in valid range [0, 64]
    if (empties < 0) empties = 0;
    if (empties > 64) empties = 64;
    
    // Game phase-based time allocation
    if (empties > 50) {
        // Opening: use 15% of time
        return static_cast<int>(total_time_ms * 0.15);
    } else if (empties > 20) {
        // Midgame: use 30% of time
        return static_cast<int>(total_time_ms * 0.30);
    } else {
        // Endgame: use 55% of time (more critical)
        return static_cast<int>(total_time_ms * 0.55);
    }
}

bool MinimaxEngine::time_exceeded() const {
    if (time_limit_ms_ <= 0) return false;
    
    using Clock = std::chrono::high_resolution_clock;
    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - search_start_);
    
    return elapsed.count() >= time_limit_ms_;
}

// Week 6: Killer moves
void MinimaxEngine::update_killer(int move, int ply) {
    if (ply < 0 || ply >= MAX_DEPTH) return;
    
    // Don't update if move is already killer1
    if (killer1_[ply] == move) return;
    
    // Shift: killer2 becomes killer1, new move becomes killer1
    killer2_[ply] = killer1_[ply];
    killer1_[ply] = move;
}

int MinimaxEngine::get_killer_bonus(int move, int ply) const {
    if (ply < 0 || ply >= MAX_DEPTH) return 0;
    
    if (killer1_[ply] == move) return 1000;
    if (killer2_[ply] == move) return 500;
    return 0;
}

void MinimaxEngine::clear_killers() {
    killer1_.fill(-1);
    killer2_.fill(-1);
}

// Week 6: Move ordering
std::vector<int> MinimaxEngine::order_moves(
    const reversi::core::Board& board, 
    const std::vector<int>& moves) const 
{
    if (moves.empty()) return moves;
    
    // Create move-score pairs
    std::vector<std::pair<int, int>> move_scores;
    move_scores.reserve(moves.size());
    
    // Get TT best move (if available)
    int tt_best_move = -1;
    if (config_.use_transposition) {
        uint64_t hash = board.hash();
        TTEntry* entry = tt_.probe(hash);
        if (entry && entry->best_move >= 0 && entry->best_move < 64) {
            tt_best_move = entry->best_move;
        }
    }
    
    // Pre-calculate flip counts to avoid repeated Board copies
    // Use const reference and calc_flip which doesn't modify board
    for (int move : moves) {
        int score = 0;
        
        // TT best move gets highest priority
        if (move == tt_best_move) {
            score += 10000;
        }
        
        // Killer moves get bonus
        if (config_.use_killer_moves) {
            score += get_killer_bonus(move, current_ply_);
        }
        
        // Evaluation-based ordering (simple heuristic)
        // Moves that flip more pieces are generally better
        // Use calc_flip which doesn't modify the board (const method)
        uint64_t flip_mask = board.calc_flip(move);
        int flip_count = std::popcount(flip_mask);
        score += flip_count * 10;
        
        move_scores.emplace_back(move, score);
    }
    
    // Sort by score (descending)
    std::sort(move_scores.begin(), move_scores.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
    
    // Extract ordered moves
    std::vector<int> ordered;
    ordered.reserve(moves.size());
    for (const auto& [move, score] : move_scores) {
        ordered.push_back(move);
    }
    
    return ordered;
}

} // namespace reversi::ai

