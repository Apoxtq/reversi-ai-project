/*
 * MCTSEngine.hpp - Monte Carlo Tree Search engine
 * COMP390 Honours Year Project
 * 
 * Monte Carlo Tree Search implementation with:
 * - UCB1 selection strategy
 * - Tree expansion and simulation
 * - Heuristic playout policy
 * - Backpropagation and statistics
 * 
 * Performance target: ~200K simulations/second
 */

#pragma once

#include "core/Board.hpp"
#include "core/Move.hpp"
#include "ai/AIStrategy.hpp"
#include <limits>
#include <chrono>
#include <cstdint>
#include <vector>
#include <memory>
#include <random>
#include <string>
#include <algorithm>

namespace reversi::ai {

/**
 * @brief Monte Carlo Tree Search engine for Reversi
 * 
 * Implements standard MCTS algorithm with UCB1 selection,
 * tree expansion, simulation (playout), and backpropagation.
 * 
 * Algorithm phases:
 * 1. Selection: Traverse tree using UCB1 until leaf node
 * 2. Expansion: Add new child node if not terminal
 * 3. Simulation: Heuristic playout to terminal state
 * 4. Backpropagation: Update statistics up the tree
 * 
 * @performance Target: ~200K simulations/second
 * @memory Efficient node management with std::unique_ptr
 */
class MCTSEngine : public AIStrategy {
public:
    /**
     * @brief MCTS configuration
     */
    struct Config {
        int max_simulations = 10000;      ///< Maximum simulations per move
        int max_time_ms = 5000;           ///< Maximum time limit (milliseconds)
        double ucb1_c = 1.414213562;      ///< UCB1 exploration constant (√2)
        bool use_heuristic_playout = true; ///< Use heuristic instead of pure random
        double playout_heuristic_weight = 0.3; ///< Top 30% moves in playout
        
        Config() = default;
        Config(int sims, int time_ms, double c = 1.414)
            : max_simulations(sims), max_time_ms(time_ms), ucb1_c(c) {}
    };
    
    /**
     * @brief Extended search statistics for MCTS
     */
    struct MCTSStats : public SearchStats {
        int simulations_performed = 0;     ///< Total simulations executed
        int tree_nodes_created = 0;        ///< Total nodes in search tree
        int max_tree_depth = 0;             ///< Maximum depth reached
        double average_playout_length = 0.0; ///< Average moves per playout
        double win_rate = 0.0;              ///< Win rate from current player's perspective
        std::vector<int> move_visit_counts;   ///< Visit counts for each legal move
        std::vector<double> move_win_rates;   ///< Win rates for each legal move
        
        void reset() override {
            SearchStats::reset();
            simulations_performed = 0;
            tree_nodes_created = 0;
            max_tree_depth = 0;
            average_playout_length = 0.0;
            win_rate = 0.0;
            move_visit_counts.clear();
            move_win_rates.clear();
        }
    };
    
    // ==================== Constructors ====================
    
    /** @brief Default constructor with default configuration */
    MCTSEngine();
    
    /** @brief Constructor with custom configuration */
    explicit MCTSEngine(const Config& config);
    
    // ==================== AIStrategy Interface ====================
    
    /** @brief Find best move using MCTS */
    core::Move find_best_move(const core::Board& board, 
                              const SearchLimits& limits) override;
    
    /** @brief Get engine name */
    std::string get_name() const override { return "MCTS"; }
    
    /** @brief Get search statistics */
    const SearchStats& get_stats() const override {
        return static_cast<const SearchStats&>(stats_);
    }
    
    /** @brief Reset internal state */
    void reset() override;
    
    // ==================== MCTS-Specific Methods ====================
    
    /** @brief Get detailed MCTS statistics */
    const MCTSStats& get_mcts_stats() const { return stats_; }
    
    /** @brief Update configuration */
    void set_config(const Config& config) { config_ = config; }
    
    /** @brief Get current configuration */
    const Config& get_config() const { return config_; }

private:
    // ==================== MCTS Tree Node ====================
    
    /**
     * @brief MCTS search tree node
     * 
     * Each node represents a game position and stores:
     * - Board state
     * - Visit count and win statistics
     * - Child nodes (legal moves)
     * - Parent reference for backpropagation
     */
    struct Node {
        core::Board board_state;      ///< Board position at this node
        int visits = 0;               ///< Number of times this node was visited
        double wins = 0.0;            ///< Cumulative wins (from current player's perspective)
        std::vector<std::unique_ptr<Node>> children; ///< Child nodes (legal moves)
        Node* parent = nullptr;       ///< Parent node (for backpropagation)
        int move = -1;                ///< Move that led to this node (-1 for root)
        bool is_terminal = false;      ///< Whether this is a terminal position
        bool is_expanded = false;      ///< Whether children have been generated
        
        // ==================== Node Operations ====================
        
        /**
         * @brief Calculate UCB1 value for this node
         * @param exploration_c UCB1 exploration constant
         * @param parent_visits Total visits of parent node
         * @return UCB1 value (exploitation + exploration)
         */
        double ucb1_value(double exploration_c, int parent_visits) const {
            if (visits == 0) {
                return std::numeric_limits<double>::max(); // Unvisited nodes prioritized
            }
            
            double exploitation = wins / visits;  // Average win rate
            double exploration = exploration_c * 
                std::sqrt(std::log(parent_visits + 1) / visits);
            
            return exploitation + exploration;
        }
        
        /**
         * @brief Select best child using UCB1
         * @param exploration_c UCB1 exploration constant
         * @return Best child node according to UCB1
         */
        Node* select_best_child(double exploration_c) const {
            if (children.empty()) return nullptr;
            
            Node* best = nullptr;
            double best_value = std::numeric_limits<double>::lowest();
            
            for (const auto& child : children) {
                double value = child->ucb1_value(exploration_c, visits);
                if (value > best_value) {
                    best_value = value;
                    best = child.get();
                }
            }
            
            return best;
        }
        
        /**
         * @brief Expand node by creating children for all legal moves
         * @param board Current board state
         */
        void expand(const core::Board& board) {
            if (is_expanded || is_terminal) return;
            
            std::vector<int> legal_moves;
            board.get_legal_moves(legal_moves);
            
            if (legal_moves.empty()) {
                is_terminal = true;
                return;
            }
            
            children.reserve(legal_moves.size());
            for (int move : legal_moves) {
                auto child = std::make_unique<Node>();
                child->board_state = board;
                child->board_state.make_move(move);
                child->parent = this;
                child->move = move;
                child->is_terminal = child->board_state.is_terminal();
                
                children.push_back(std::move(child));
            }
            
            is_expanded = true;
        }
        
        /**
         * @brief Update node statistics (backpropagation)
         * @param result Simulation result (1.0 = win, 0.0 = loss, 0.5 = draw)
         */
        void backpropagate(double result) {
            visits++;
            wins += result;
            
            // Update parent recursively
            if (parent != nullptr) {
                parent->backpropagate(1.0 - result); // Invert for opponent's perspective
            }
        }
        
        /**
         * @brief Check if node is a leaf
         */
        bool is_leaf() const { 
            return children.empty() || !is_expanded; 
        }
    };
    
    // ==================== MCTS Algorithm Phases ====================
    
    /**
     * @brief Phase 1: Selection - Traverse tree using UCB1
     * @param root Root node of search tree
     * @return Leaf node reached by UCB1 selection
     */
    Node* selection(Node* root);
    
    /**
     * @brief Phase 2: Expansion - Add new child node
     * @param leaf Leaf node to expand
     * @return Newly expanded node (or leaf if terminal)
     */
    Node* expansion(Node* leaf);
    
    /**
     * @brief Phase 3: Simulation - Play heuristic game to terminal
     * @param node Node to start simulation from
     * @return Result from current player's perspective (1.0 = win, 0.0 = loss)
     */
    double simulation(Node* node);
    
    /**
     * @brief Phase 4: Backpropagation - Update statistics up the tree
     * @param node Node to start backpropagation from
     * @param result Simulation result
     */
    void backpropagation(Node* node, double result);
    
    // ==================== Helper Methods ====================
    
    /**
     * @brief Random playout from given board position
     * @param board Starting board position
     * @return Result from current player's perspective (1 = win, 0 = loss)
     */
    int random_playout(const core::Board& board);
    
    /**
     * @brief Heuristic playout (improved over pure random)
     * @param board Starting board position
     * @return Result from current player's perspective (1 = win, 0 = loss)
     */
    int heuristic_playout(const core::Board& board);
    
    /**
     * @brief Evaluate move using lightweight heuristics
     * @param board Current board state
     * @param move Move to evaluate
     * @return Heuristic score (higher = better)
     */
    int evaluate_move_heuristic(const core::Board& board, int move);
    
    /**
     * @brief Select best move (most visited child)
     * @param root Root node of search tree
     * @return Best move found
     */
    core::Move select_best_move(Node* root);
    
    /**
     * @brief Count total nodes in tree
     * @param node Root node
     * @return Total node count
     */
    int count_tree_nodes(Node* node) const;
    
    /**
     * @brief Calculate maximum tree depth
     * @param node Current node
     * @param depth Current depth
     * @return Maximum depth
     */
    int calculate_max_depth(Node* node, int depth) const;
    
    // ==================== Member Variables ====================
    
    Config config_;                    ///< MCTS configuration
    MCTSStats stats_;                 ///< Search statistics
    std::unique_ptr<Node> root_;       ///< Root of search tree
    std::mt19937 rng_;                 ///< Random number generator
    
    // Performance optimization
    static constexpr int TIME_CHECK_INTERVAL = 100; ///< Check time every N simulations
    int total_playout_moves_ = 0;      ///< Total moves in all playouts (for average)
};

} // namespace reversi::ai

