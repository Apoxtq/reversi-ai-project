/*
 * MCTSEngine.cpp - Monte Carlo Tree Search engine implementation
 * COMP390 Honours Year Project
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "ai/MCTSEngine.hpp"
#include <chrono>
#include <algorithm>
#include <cmath>
#include <random>

namespace reversi::ai {

// ==================== Constructors ====================

MCTSEngine::MCTSEngine() 
    : config_()
    , rng_(std::random_device{}())
{
    stats_.reset();
}

MCTSEngine::MCTSEngine(const Config& config)
    : config_(config)
    , rng_(std::random_device{}())
{
    stats_.reset();
}

// ==================== AIStrategy Interface ====================

void MCTSEngine::reset() {
    stats_.reset();
    root_.reset();
    total_playout_moves_ = 0;
}

core::Move MCTSEngine::find_best_move(const core::Board& board, 
                                      const SearchLimits& limits) {
    stats_.reset();
    total_playout_moves_ = 0;
    auto start_time = std::chrono::steady_clock::now();
    
    // Create root node
    root_ = std::make_unique<Node>();
    root_->board_state = board;
    root_->expand(board);
    
    if (root_->is_terminal) {
        // Terminal position - return pass move
        return core::Move(core::Move::PASS);
    }
    
    // Determine search limits
    int max_sims = (limits.max_nodes > 0) ? 
        limits.max_nodes : config_.max_simulations;
    int max_time = (limits.max_time_ms > 0) ? 
        limits.max_time_ms : config_.max_time_ms;
    
    int simulations = 0;
    
    // MCTS main loop
    while (simulations < max_sims) {
        // Check time limit (optimized: check every N simulations)
        if (simulations % TIME_CHECK_INTERVAL == 0) {
            auto elapsed = std::chrono::steady_clock::now() - start_time;
            int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            if (elapsed_ms >= max_time) {
                break;
            }
        }
        
        // MCTS four phases
        Node* leaf = selection(root_.get());        // 1. Selection
        Node* expanded = expansion(leaf);            // 2. Expansion
        double result = simulation(expanded);        // 3. Simulation
        backpropagation(expanded, result);          // 4. Backpropagation
        
        simulations++;
        stats_.simulations_performed++;
    }
    
    // Select best move (most visited child)
    core::Move best_move = select_best_move(root_.get());
    
    // Update statistics
    auto elapsed = std::chrono::steady_clock::now() - start_time;
    stats_.time_elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    stats_.tree_nodes_created = count_tree_nodes(root_.get());
    stats_.nodes_searched = stats_.tree_nodes_created;
    stats_.nodes_per_second = (stats_.time_elapsed_ms > 0) ?
        (stats_.simulations_performed * 1000.0 / stats_.time_elapsed_ms) : 0.0;
    stats_.average_playout_length = (stats_.simulations_performed > 0) ?
        (total_playout_moves_ / static_cast<double>(stats_.simulations_performed)) : 0.0;
    stats_.max_tree_depth = calculate_max_depth(root_.get(), 0);
    
    // Calculate win rate and move statistics
    if (!root_->children.empty()) {
        stats_.win_rate = (root_->visits > 0) ? (root_->wins / root_->visits) : 0.0;
        for (const auto& child : root_->children) {
            stats_.move_visit_counts.push_back(child->visits);
            stats_.move_win_rates.push_back((child->visits > 0) ? 
                (child->wins / child->visits) : 0.0);
        }
    }
    
    return best_move;
}

// ==================== MCTS Algorithm Phases ====================

MCTSEngine::Node* MCTSEngine::selection(Node* root) {
    Node* current = root;
    
    // Traverse tree using UCB1 until we reach a leaf
    while (!current->is_leaf()) {
        current = current->select_best_child(config_.ucb1_c);
        if (current == nullptr) {
            // Should not happen, but handle gracefully
            break;
        }
    }
    
    return current;
}

MCTSEngine::Node* MCTSEngine::expansion(Node* leaf) {
    if (leaf->is_terminal) {
        return leaf; // Cannot expand terminal nodes
    }
    
    if (!leaf->is_expanded) {
        leaf->expand(leaf->board_state);
    }
    
    // Return first unvisited child, or random child if all visited
    if (!leaf->children.empty()) {
        // Find unvisited child
        for (auto& child : leaf->children) {
            if (child->visits == 0) {
                return child.get();
            }
        }
        // All children visited, return random child
        std::uniform_int_distribution<size_t> dist(0, leaf->children.size() - 1);
        return leaf->children[dist(rng_)].get();
    }
    
    return leaf;
}

double MCTSEngine::simulation(Node* node) {
    if (node->is_terminal) {
        // Terminal node - return actual game result
        int winner = node->board_state.get_winner();
        if (winner == 1) return 1.0;      // Current player wins
        if (winner == -1) return 0.0;     // Opponent wins
        return 0.0; // Draw treated as loss (conservative)
    }
    
    // Run playout
    int result;
    if (config_.use_heuristic_playout) {
        result = heuristic_playout(node->board_state);
    } else {
        result = random_playout(node->board_state);
    }
    
    return static_cast<double>(result);
}

void MCTSEngine::backpropagation(Node* node, double result) {
    node->backpropagate(result);
}

// ==================== Playout Methods ====================

int MCTSEngine::random_playout(const core::Board& board) {
    core::Board current = board;
    int move_count = 0;
    
    while (!current.is_terminal()) {
        std::vector<int> legal_moves;
        current.get_legal_moves(legal_moves);
        
        if (legal_moves.empty()) {
            current.pass();
            continue;
        }
        
        // Random selection
        std::uniform_int_distribution<size_t> dist(0, legal_moves.size() - 1);
        int random_idx = dist(rng_);
        current.make_move(legal_moves[random_idx]);
        move_count++;
    }
    
    total_playout_moves_ += move_count;
    
    // Return result from original player's perspective
    int player_count = current.count_player();
    int opponent_count = current.count_opponent();
    
    if (player_count > opponent_count) return 1;
    if (player_count < opponent_count) return 0;
    return 0; // Draw treated as loss (conservative)
}

int MCTSEngine::heuristic_playout(const core::Board& board) {
    core::Board current = board;
    int move_count = 0;
    
    while (!current.is_terminal()) {
        std::vector<int> legal_moves;
        current.get_legal_moves(legal_moves);
        
        if (legal_moves.empty()) {
            current.pass();
            continue;
        }
        
        // Score all legal moves
        std::vector<std::pair<int, int>> scored_moves; // (score, move)
        scored_moves.reserve(legal_moves.size());
        
        for (int move : legal_moves) {
            int score = evaluate_move_heuristic(current, move);
            scored_moves.push_back({score, move});
        }
        
        // Sort by score (descending)
        std::sort(scored_moves.rbegin(), scored_moves.rend());
        
        // Select from top N% moves (with randomness)
        int top_n = std::max(1, static_cast<int>(scored_moves.size() * 
            config_.playout_heuristic_weight));
        std::uniform_int_distribution<size_t> dist(0, top_n - 1);
        int random_idx = dist(rng_);
        
        current.make_move(scored_moves[random_idx].second);
        move_count++;
    }
    
    total_playout_moves_ += move_count;
    
    // Return result from original player's perspective
    int player_count = current.count_player();
    int opponent_count = current.count_opponent();
    
    if (player_count > opponent_count) return 1;
    if (player_count < opponent_count) return 0;
    return 0; // Draw treated as loss (conservative)
}

int MCTSEngine::evaluate_move_heuristic(const core::Board& board, int move) {
    int score = 0;
    
    // Position-based heuristics
    static constexpr int CORNER_BONUS = 100;
    static constexpr int EDGE_BONUS = 10;
    static constexpr int X_SQUARE_PENALTY = -50; // Dangerous X-square
    static constexpr int C_SQUARE_PENALTY = -20; // Risky C-square
    
    int row = move / 8;
    int col = move % 8;
    
    // Corner positions (A1, H1, A8, H8)
    if ((row == 0 || row == 7) && (col == 0 || col == 7)) {
        score += CORNER_BONUS;
    }
    // X-squares (B2, G2, B7, G7) - dangerous
    else if ((row == 1 || row == 6) && (col == 1 || col == 6)) {
        score += X_SQUARE_PENALTY;
    }
    // C-squares (A2, B1, etc.) - risky
    else if ((row == 0 || row == 7) || (col == 0 || col == 7)) {
        if ((row <= 1 || row >= 6) && (col <= 1 || col >= 6)) {
            score += C_SQUARE_PENALTY;
        } else {
            score += EDGE_BONUS;
        }
    }
    
    // Mobility: prefer moves that give opponent fewer options
    core::Board test_board = board;
    test_board.make_move(move);
    std::vector<int> opponent_moves;
    test_board.get_legal_moves(opponent_moves);
    int opponent_mobility = opponent_moves.size();
    score -= opponent_mobility * 2; // Lower opponent mobility = better
    
    return score;
}

// ==================== Helper Methods ====================

core::Move MCTSEngine::select_best_move(Node* root) {
    if (root->children.empty()) {
        return core::Move(core::Move::PASS);
    }
    
    // Select most visited child
    Node* best = nullptr;
    int max_visits = -1;
    
    for (const auto& child : root->children) {
        if (child->visits > max_visits) {
            max_visits = child->visits;
            best = child.get();
        }
    }
    
    if (best == nullptr || best->move < 0) {
        return core::Move(core::Move::PASS);
    }
    
    return core::Move(best->move);
}

int MCTSEngine::count_tree_nodes(Node* node) const {
    if (node == nullptr) return 0;
    
    int count = 1; // Count this node
    for (const auto& child : node->children) {
        count += count_tree_nodes(child.get());
    }
    
    return count;
}

int MCTSEngine::calculate_max_depth(Node* node, int depth) const {
    if (node == nullptr) return depth;
    
    int max_depth = depth;
    for (const auto& child : node->children) {
        int child_depth = calculate_max_depth(child.get(), depth + 1);
        if (child_depth > max_depth) {
            max_depth = child_depth;
        }
    }
    
    return max_depth;
}

} // namespace reversi::ai

