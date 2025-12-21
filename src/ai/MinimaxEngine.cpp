/*
 * MinimaxEngine.cpp - Implementation of Minimax search
 * COMP390 Honours Year Project
 */

#include "ai/MinimaxEngine.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace reversi::ai {

// Lightweight static positional weights to support fast move ordering.
// Values chosen to prefer corners and edges and penalize squares adjacent to corners.
static constexpr int POSITION_WEIGHTS[64] = {
    1000, -250,  50,  50,  50,  50, -250, 1000,
   -250, -500,  10,  10,  10,  10, -500, -250,
     50,   10,  20,  20,  20,  20,   10,   50,
     50,   10,  20,  20,  20,  20,   10,   50,
     50,   10,  20,  20,  20,  20,   10,   50,
     50,   10,  20,  20,  20,  20,   10,   50,
   -250, -500,  10,  10,  10,  10, -500, -250,
   1000, -250,  50,  50,  50,  50, -250, 1000
};

// Small move list threshold for stack-buffer optimization
static constexpr size_t SMALL_MOVE_LIMIT = 16;

// Infinity constant (avoid overflow in negation)
constexpr int INF = std::numeric_limits<int>::max() / 2;

// Constructor implementations
MinimaxEngine::MinimaxEngine() : config_(), tt_(config_.tt_size_bits) {
    clear_killers();
    last_stats_.reset();
    pvs_zero_window_failures_ = 0;
    pvs_researches_ = 0;
    pvs_zero_window_beta_cutoffs_ = 0;
    pvs_zero_window_failures_per_ply_.fill(0);
    pvs_researches_per_ply_.fill(0);
    pvs_zero_window_beta_cutoffs_per_ply_.fill(0);
    moves_scratch_.reserve(64);
    move_scores_scratch_.reserve(64);
    history_table_.fill(0);
}

MinimaxEngine::MinimaxEngine(const Config& config) 
    : config_(config), tt_(config.tt_size_bits) {
    clear_killers();
    last_stats_.reset();
    pvs_zero_window_failures_ = 0;
    pvs_researches_ = 0;
    pvs_zero_window_beta_cutoffs_ = 0;
    pvs_zero_window_failures_per_ply_.fill(0);
    pvs_researches_per_ply_.fill(0);
    pvs_zero_window_beta_cutoffs_per_ply_.fill(0);
    moves_scratch_.reserve(64);
    history_table_.fill(0);
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
    // Slightly decay history heuristic to avoid unbounded growth across moves.
    decay_history();
    
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
    
    moves_scratch_.clear();
    board.get_legal_moves(moves_scratch_);
    const auto& moves = moves_scratch_;
    int small_moves_stack[SMALL_MOVE_LIMIT];
    const int* moves_ptr = nullptr;
    size_t moves_n = moves.size();
    if (moves_n == 0) {
        auto end = Clock::now();
        double time_ms = std::chrono::duration<double, std::milli>(end - search_start_).count();
        return {-1, 0, 0, 0, time_ms};
    }
    if (moves_n <= SMALL_MOVE_LIMIT) {
        for (size_t _i = 0; _i < moves_n; ++_i) small_moves_stack[_i] = moves[_i];
        moves_ptr = small_moves_stack;
    } else {
        moves_ptr = moves.data();
    }
    
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
    
    // Root level: search all legal moves (use one mutable tmp board to avoid per-move copies)
    reversi::core::Board tmp_root = board;
    for (size_t _mi = 0; _mi < moves_n; ++_mi) {
        int move = moves_ptr[_mi];
        if (time_exceeded()) break;

        uint64_t prev_p = tmp_root.get_player_bb();
        uint64_t prev_o = tmp_root.get_opponent_bb();
        uint64_t prev_hash = tmp_root.hash();
        tmp_root.apply_move_no_history(move);

        // Search opponent's response
        int score;
        if (config_.use_pvs) {
            score = -pvs(tmp_root, config_.max_depth - 1, -beta, -alpha, false);
        } else {
            score = -negamax(tmp_root, config_.max_depth - 1, -beta, -alpha);
        }
        tmp_root.restore_state(prev_p, prev_o, prev_hash);
        
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
    
    // Print lightweight PVS diagnostics for tuning (only when PVS enabled)
    if (config_.use_pvs) {
        // Reset counters for next search (diagnostics retained internally; no console IO)
        pvs_zero_window_failures_ = 0;
        pvs_researches_ = 0;
        pvs_zero_window_beta_cutoffs_ = 0;
        pvs_zero_window_failures_per_ply_.fill(0);
        pvs_researches_per_ply_.fill(0);
        pvs_zero_window_beta_cutoffs_per_ply_.fill(0);
    }

    return {
        best_move,
        best_score,
        nodes_searched_,
        config_.max_depth,
        time_ms
    };
}

inline int MinimaxEngine::negamax(
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
    
    // Get legal moves for current player (reuse member buffer to avoid allocations)
    moves_scratch_.clear();
    moves_scratch_.reserve(32);
    board.get_legal_moves(moves_scratch_);
    const auto& moves = moves_scratch_;
    
    // No legal moves: must pass
    if (moves.empty()) {
        reversi::core::Board next = board;
        next.pass();
        // Opponent's turn, negate score and swap alpha-beta
        int score = -negamax(next, depth - 1, -beta, -alpha);
        --current_ply_;
        return score;
    }
    
    // Cache frequently-used config flags locally to reduce member access and branches
    const bool use_alpha = config_.use_alpha_beta;
    const bool use_killer = config_.use_killer_moves;
    const bool use_trans = config_.use_transposition;

    // Move ordering: use comprehensive ordering if enabled
    std::vector<int> ordered_moves;
    if (use_killer || use_trans) {
        ordered_moves = order_moves(board, moves);
    } else {
        ordered_moves = moves;
    }
    
    // Negamax recursion
    int best_score = -INF;
    int best_move = ordered_moves[0];
    int original_alpha = alpha;
    
    for (int move : ordered_moves) {
        // Apply move in-place (fast path) and restore after recursion
        uint64_t prev_p = board.get_player_bb();
        uint64_t prev_o = board.get_opponent_bb();
        uint64_t prev_hash = board.hash();
        board.apply_move_no_history(move);
        // Recursive search (opponent's turn, negate score)
        int score = -negamax(board, depth - 1, -beta, -alpha);
        // Restore board
        board.restore_state(prev_p, prev_o, prev_hash);
        
        // Update best score and move (branch predicted)
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }

        // Alpha-Beta pruning (use local flag)
        if (use_alpha) {
            if (score > alpha) alpha = score;
            if (alpha >= beta) {
                // Beta cutoff: opponent won't allow this position
                // Update killer move (use local flag to avoid member access)
                if (use_killer) {
                    update_killer(move, current_ply_);
                }
                    // Update history heuristic for this move
                    update_history(move, depth);
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
        
        // Use aspiration windows if enabled and we have a predicted score.
        // Avoid using aspiration at very shallow depths during iterative deepening to reduce noisy re-searches.
        if (config_.use_aspiration && depth > 2 && predicted_score != 0) {
            result = aspiration_search(board, depth, predicted_score);
        } else {
            // Standard search
            int alpha = -INF;
            int beta = INF;
            
            // Compute root ordering per-depth when doing iterative deepening so TT and previous searches
            // can update move ordering; for non-ID callers this will just run once.
            moves_scratch_.clear();
            board.get_legal_moves(moves_scratch_);
            std::vector<int> root_ordered = order_moves(board, moves_scratch_);
            const auto& moves = root_ordered;
            int small_moves_stack[SMALL_MOVE_LIMIT];
            const int* moves_ptr = nullptr;
            size_t moves_n = moves.size();
            if (moves_n == 0) {
                break;
            } else if (moves_n <= SMALL_MOVE_LIMIT) {
                for (size_t _i = 0; _i < moves_n; ++_i) small_moves_stack[_i] = moves[_i];
                moves_ptr = small_moves_stack;
            } else {
                moves_ptr = moves.data();
            }
            
            int best_move = moves[0];
            int best_score = -INF;
            
            for (size_t _mi = 0; _mi < moves_n; ++_mi) {
                int move = moves_ptr[_mi];
                if (time_exceeded()) break;

                reversi::core::Board tmp = board;
                tmp.apply_move_no_history(move);

                int score;
                if (config_.use_pvs) {
                    score = -pvs(tmp, depth - 1, -beta, -alpha, false);
                } else {
                    score = -negamax(tmp, depth - 1, -beta, -alpha);
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
    
    // For shallow depths aspiration windows can be too narrow and cause noisy re-searches.
    int window = config_.aspiration_window;
    if (depth <= 5) {
        // widen window at shallow depths to reduce re-search noise
        window = std::max(window, 200);
    }
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
inline int MinimaxEngine::pvs(
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

    // Fallback: if this ply has shown many zero-window failures, skip PVS here
    // Use configurable per-ply threshold to fall back to negamax when PVS shows many failures.
    const int PVS_FAILURE_THRESHOLD = config_.pvs_failure_threshold;
    if (config_.use_pvs && pvs_zero_window_failures_per_ply_[current_ply_] > PVS_FAILURE_THRESHOLD) {
        // Use standard negamax (full-window) at this node to avoid repeated zero-window re-searches
        int fallback_score = negamax(board, depth, alpha, beta);
        --current_ply_;
        return fallback_score;
    }
    
    // Get legal moves (reuse buffer to avoid allocations)
    moves_scratch_.clear();
    board.get_legal_moves(moves_scratch_);
    const auto& moves = moves_scratch_;

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
    
    // Cache config flags locally
    const bool use_alpha = config_.use_alpha_beta;
    const bool use_killer = config_.use_killer_moves;

    // First move: full window search
    {
        // Apply first move in-place (fast path) and restore after search
        uint64_t prev_p = board.get_player_bb();
        uint64_t prev_o = board.get_opponent_bb();
        uint64_t prev_hash = board.hash();
        board.apply_move_no_history(ordered_moves[0]);
        best_score = -pvs(board, depth - 1, -beta, -alpha, true);
        best_move = ordered_moves[0];
        // Restore board
        board.restore_state(prev_p, prev_o, prev_hash);
        
        if (use_alpha) {
            if (best_score > alpha) alpha = best_score;
            if (alpha >= beta) {
                // Beta cutoff
                if (use_killer) {
                    update_killer(ordered_moves[0], current_ply_);
                }
                    update_history(ordered_moves[0], depth);
                --current_ply_;
                return best_score;
            }
        }
    }
    
    // Subsequent moves: zero window search (null window)
    for (size_t i = 1; i < ordered_moves.size(); ++i) {
        if (time_exceeded()) break;
        
        // Apply move in-place and restore after
        uint64_t prev_p = board.get_player_bb();
        uint64_t prev_o = board.get_opponent_bb();
        uint64_t prev_hash = board.hash();
        board.apply_move_no_history(ordered_moves[i]);
        // Zero window search: assume this move is not better
        // Search with window [alpha, alpha+1] to test if score > alpha
        int score = -pvs(board, depth - 1, -alpha - 1, -alpha, false);
        
        // If zero window fails, decide whether to re-search.
    // Use runtime-configurable PVS research margin
    const int PVS_RESEARCH_MARGIN = config_.pvs_research_margin;
            if (score >= beta) {
            // Zero-window exceeded beta: immediate beta-cut
            ++pvs_zero_window_failures_;
            if (current_ply_ >= 0 && current_ply_ < MAX_DEPTH) {
                ++pvs_zero_window_failures_per_ply_[current_ply_];
                ++pvs_zero_window_beta_cutoffs_per_ply_[current_ply_];
            }
            ++pvs_zero_window_beta_cutoffs_;
            if (config_.use_killer_moves) {
                update_killer(ordered_moves[i], current_ply_);
            }
                // Update history heuristic for this move causing beta cutoff
                update_history(ordered_moves[i], depth);
            best_score = score;
            best_move = ordered_moves[i];
            break;
        } else if (score > alpha + PVS_RESEARCH_MARGIN) {
            // Zero-window failed with sufficient margin: re-search with full window
            ++pvs_zero_window_failures_;
            if (current_ply_ >= 0 && current_ply_ < MAX_DEPTH) {
                ++pvs_zero_window_failures_per_ply_[current_ply_];
            }
            ++pvs_researches_;
            if (current_ply_ >= 0 && current_ply_ < MAX_DEPTH) {
                ++pvs_researches_per_ply_[current_ply_];
            }
            score = -pvs(board, depth - 1, -beta, -alpha, true);
        }
        
        if (score > best_score) {
            best_score = score;
            best_move = ordered_moves[i];
        }
        // Restore board after recursive search
        board.restore_state(prev_p, prev_o, prev_hash);
        
        if (use_alpha) {
            if (score > alpha) alpha = score;
            if (alpha >= beta) {
                // Beta cutoff
                if (use_killer) {
                    update_killer(ordered_moves[i], current_ply_);
                }
                    update_history(ordered_moves[i], depth);
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

MinimaxEngine::PVSDiagnostics MinimaxEngine::get_pvs_diagnostics() const {
    PVSDiagnostics d;
    d.zero_window_failures = pvs_zero_window_failures_;
    d.researches = pvs_researches_;
    d.zero_window_beta_cutoffs = pvs_zero_window_beta_cutoffs_;
    d.failures_per_ply.assign(pvs_zero_window_failures_per_ply_.begin(), pvs_zero_window_failures_per_ply_.end());
    d.researches_per_ply.assign(pvs_researches_per_ply_.begin(), pvs_researches_per_ply_.end());
    d.beta_cutoffs_per_ply.assign(pvs_zero_window_beta_cutoffs_per_ply_.begin(), pvs_zero_window_beta_cutoffs_per_ply_.end());
    return d;
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

// Update history heuristic for a move that caused a beta-cutoff.
void MinimaxEngine::update_history(int move, int depth) {
    if (move < 0 || move >= 64) return;
    // Heuristic: bigger depth increments history more (prefer moves that cause deep cutoffs)
    history_table_[move] += (depth * depth + 1);
}

// Apply decay to the history table to slowly forget old entries.
void MinimaxEngine::decay_history() {
    // Gentle decay: subtract 1/8 of the current value (v -= v>>3), avoids rapid zeroing.
    for (int &v : history_table_) {
        v -= (v >> 3);
    }
}

// Week 6: Move ordering
std::vector<int> MinimaxEngine::order_moves(
    const reversi::core::Board& board, 
    const std::vector<int>& moves) const 
{
    if (moves.empty()) return moves;
    
    // Create move-score pairs using reusable scratch vector to avoid allocations
    move_scores_scratch_.clear();
    move_scores_scratch_.reserve(moves.size());
    
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
    bool tt_move_used = false;
    // Simple conservative ordering: TT, killer, positional weight, flip count.
    move_scores_scratch_.clear();
    move_scores_scratch_.reserve(moves.size());
    for (int move : moves) {
        int score = 0;
        if (move == tt_best_move) {
            score += 10000;
            tt_move_used = true;
        }
        if (config_.use_killer_moves) {
            score += get_killer_bonus(move, current_ply_);
        }
        int pos_weight = POSITION_WEIGHTS[move];
        score += pos_weight;
        uint64_t flip_mask = board.calc_flip(move);
        int flip_count = static_cast<int>(std::popcount(flip_mask));
        score += flip_count * 4;
        move_scores_scratch_.emplace_back(move, score);
    }
    
    // Do a very cheap partial refinement on a small number of top candidates to improve PVS ordering.
    // At root we refine more candidates (higher potential benefit); in deeper plies keep it minimal.
    int top_k = 0;
    if (current_ply_ == 0) {
        // Be conservative at root: only refine top-1 to avoid noisy re-ordering.
        top_k = static_cast<int>(std::min<size_t>(1, move_scores_scratch_.size()));
    } else if (current_ply_ >= 3) {
        top_k = 1;
    } else {
        top_k = 2;
    }
    if (top_k > 0 && move_scores_scratch_.size() > 1) {
        if (top_k < static_cast<int>(move_scores_scratch_.size())) {
            std::partial_sort(move_scores_scratch_.begin(), move_scores_scratch_.begin() + top_k, move_scores_scratch_.end(),
                [](const auto& a, const auto& b){ return a.second > b.second; });
        } else {
            // small set: full sort is fine
            std::sort(move_scores_scratch_.begin(), move_scores_scratch_.end(),
                [](const auto& a, const auto& b){ return a.second > b.second; });
        }

        // Evaluate the top_k candidates with a lightweight evaluation to improve ordering.
        for (int i = 0; i < top_k; ++i) {
            int mv = move_scores_scratch_[i].first;
            // quick deepening: evaluate resulting position to improve ordering
            reversi::core::Board next = board;
            next.make_move(mv);
            int eval_score = Evaluator::evaluate(next);
            // amplify evaluation to influence ordering but keep cost low (use shift instead of mul)
            move_scores_scratch_[i].second += (eval_score << 3); // *8
        }
    }

    // Final sort by updated score (descending)
    // Sort move_scores_scratch_ descending by score.
    // Use insertion sort for small sizes to avoid allocation/call overhead of std::sort.
    size_t n = move_scores_scratch_.size();
    if (n <= 16) {
        for (size_t i = 1; i < n; ++i) {
            auto key = move_scores_scratch_[i];
            size_t j = i;
            while (j > 0 && move_scores_scratch_[j - 1].second < key.second) {
                move_scores_scratch_[j] = move_scores_scratch_[j - 1];
                --j;
            }
            move_scores_scratch_[j] = key;
        }
    } else {
        std::sort(move_scores_scratch_.begin(), move_scores_scratch_.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });
    }

    // Extract ordered moves, ensuring TT best move is placed first if present
    std::vector<int> ordered;
    ordered.reserve(moves.size());
    if (tt_best_move >= 0 && tt_move_used) {
        ordered.push_back(tt_best_move);
    }
    for (const auto& ms : move_scores_scratch_) {
        int mv = ms.first;
        if (mv == tt_best_move) continue;
        ordered.push_back(mv);
    }

    return ordered;
}

} // namespace reversi::ai

