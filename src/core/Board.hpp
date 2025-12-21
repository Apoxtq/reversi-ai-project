/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Board Class - Bitboard Implementation
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <bit>

namespace reversi {
namespace core {

/**
 * @brief High-performance bitboard-based Reversi board representation
 * 
 * Uses two 64-bit integers to represent player and opponent pieces.
 * Implements Kogge-Stone parallel scanning algorithm for optimal performance.
 * 
 * @performance Legal move generation: O(8) instead of O(512)
 * @memory 16 bytes per board state (2 × uint64_t)
 */
class Board {
public:
    uint64_t player;    ///< Bitboard for current player's pieces
    uint64_t opponent;  ///< Bitboard for opponent's pieces
    
    /**
     * @brief Cached Zobrist hash of current state (player/opponent orientation)
     * Maintained by make_move/undo_move/pass and constructors.
     */
    uint64_t hash_cache_ = 0;

    // Zobrist hashing tables (initialized once)
    static uint64_t zobrist_player[64];
    static uint64_t zobrist_opponent[64];
    static bool zobrist_initialized;
    
public:
    // ==================== Constructors ====================
    
    /** @brief Initialize board with standard Reversi starting position */
    Board();
    
    /** @brief Construct board from bitboards directly 
     *  @param p Player bitboard
     *  @param o Opponent bitboard
     */
    Board(uint64_t p, uint64_t o);
    
    /** @brief Parse board from string representation (future) */
    Board(const std::string& board_str);
    
    // ==================== Copy Operations ====================
    
    /** @brief Return copy of this board */
    Board copy() const;
    
    /** @brief Copy this board to destination pointer */
    void copy(Board* dest) const;
    
    // ==================== Move Generation ====================
    
    /** @brief Generate bitboard of all legal moves for current player
     *  @return Bitboard with 1s at legal move positions
     *  @complexity O(8) - 8 directional scans using parallel bitwise operations
     *  @algorithm Kogge-Stone parallel prefix (flood-fill)
     */
    uint64_t legal_moves() const;
    
    /** @brief Get legal moves as vector of position indices
     *  @param out_moves Output vector to populate (avoids allocation if pre-reserved)
     *  @complexity O(8 + m) where m = number of legal moves
     */
    void get_legal_moves(std::vector<int>& out_moves) const;
    
    /** @brief Legacy interface: get legal moves as new vector
     *  @deprecated Use get_legal_moves(vector&) for better performance
     */
    std::vector<int> get_legal_moves() const;
    
    // ==================== Move Execution ====================
    
    /** @brief Execute a move at given position
     *  @param pos Position index [0-63]
     *  @complexity O(8) - parallel flip calculation in 8 directions
     *  @note Automatically flips pieces and swaps player/opponent
     */
    void make_move(int pos);
    
    /** @brief Undo previous move (with move history - Week 3) */
    void undo_move(int pos);
    
    /** @brief Pass turn (swap player and opponent) */
    void pass();
    
    // ==================== Board State Queries ====================
    
    /** @brief Count current player's pieces */
    int count_player() const;
    
    /** @brief Count opponent's pieces */
    int count_opponent() const;
    
    /** @brief Check if game is over (no legal moves for both players) */
    bool is_terminal() const;
    
    /** @brief Determine winner: 1 = player, -1 = opponent, 0 = draw */
    int get_winner() const;
    
    // ==================== Utility Functions ====================
    
    /** @brief Convert board to human-readable string */
    std::string to_string() const;
    
    /** @brief Print board to stdout */
    void print() const;
    
    /** @brief Compute Zobrist hash for transposition table
     *  @return 64-bit hash value
     *  @note Zobrist hashing provides near-perfect hash distribution
     */
    uint64_t hash() const;
    
    // ==================== Fast mutation helpers (no history, low-level)
    /** @brief Get raw player bitboard (for performance-critical code) */
    uint64_t get_player_bb() const { return player; }
    /** @brief Get raw opponent bitboard (for performance-critical code) */
    uint64_t get_opponent_bb() const { return opponent; }
    /** @brief Set raw player/opponent bitboards (for fast restore) */
    void set_player_opponent(uint64_t p, uint64_t o) { player = p; opponent = o; hash_cache_ = recompute_hash(player, opponent); }
    /** @brief Apply move without recording history (fast path for search) */
    inline void apply_move_no_history(int pos) {
        if (pos < 0 || pos >= 64) return;
        uint64_t pos_mask = 1ULL << pos;
        uint64_t flipped = calc_flip(pos);
        if (flipped == 0) return;

        uint64_t prev_player = player;
        uint64_t prev_opponent = opponent;

        player |= pos_mask | flipped;
        opponent &= ~flipped;
        std::swap(player, opponent);

        // Incremental Zobrist update using local pointers
        const uint64_t* zplayer = zobrist_player;
        const uint64_t* zopponent = zobrist_opponent;
        uint64_t delta_mask1 = prev_player;
        uint64_t delta_mask2 = prev_opponent & ~flipped;
        uint64_t delta_mask = delta_mask1 | delta_mask2;
        uint64_t delta_xor = 0;
        while (delta_mask) {
            int b = std::countr_zero(delta_mask);
            delta_xor ^= zplayer[b] ^ zopponent[b];
            delta_mask &= delta_mask - 1;
        }
        uint64_t new_hash = hash_cache_ ^ delta_xor ^ zopponent[pos];
        hash_cache_ = new_hash;
    }
    /** @brief Restore previous player/opponent/hash state */
    void restore_state(uint64_t prev_player, uint64_t prev_opponent, uint64_t prev_hash) {
        player = prev_player;
        opponent = prev_opponent;
        hash_cache_ = prev_hash;
    }
    
    // ==================== Internal Optimized Algorithms ====================
    
    /** @brief Optimized legal move generation (Kogge-Stone)
     *  @complexity O(8) parallel directional scans
     */
    uint64_t calc_legal_impl() const;
    
    /** @brief Calculate flipped pieces for a move at position
     *  @param pos Position index [0-63]
     *  @return Bitboard of pieces that would be flipped
     *  @complexity O(8) parallel flip calculation
     */
    uint64_t calc_flip(int pos) const;
    
private:
    // ==================== Helper Functions ====================
    
    /** @brief Initialize Zobrist hashing tables (called once) */
    static void init_zobrist();
    
    /** @brief Recompute Zobrist hash from given bitboards (utility) */
    static uint64_t recompute_hash(uint64_t p, uint64_t o);
    
    /** @brief Shift bitboard in given direction with edge masking
     *  @param bb Bitboard to shift
     *  @param dir Direction constant
     *  @return Shifted bitboard with proper edge handling
     */
    static inline uint64_t shift_bb(uint64_t bb, int dir);
    
    /** @brief Generate potential moves in one direction (Kogge-Stone scan)
     *  @param player_bb Current player's pieces
     *  @param opponent_bb Opponent's pieces  
     *  @param dir Direction to scan
     *  @return Bitboard of legal moves in this direction
     */
    static uint64_t gen_moves_direction(uint64_t player_bb, uint64_t opponent_bb, int dir);
    
    /** @brief Calculate flips in one direction
     *  @param pos Move position
     *  @param player_bb Current player's pieces
     *  @param opponent_bb Opponent's pieces
     *  @param dir Direction to scan
     *  @return Bitboard of flipped pieces in this direction
     */
    static uint64_t calc_flips_direction(int pos, uint64_t player_bb, uint64_t opponent_bb, int dir);

    // ==================== Move History (for undo) ====================
    struct MoveRecord {
        uint64_t prev_player;
        uint64_t prev_opponent;
        uint64_t prev_hash;
        int pos;
    };
    std::vector<MoveRecord> history_;
};

} // namespace core
} // namespace reversi

