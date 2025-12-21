/*
 * TranspositionTable.hpp - Transposition table for Minimax search
 * COMP390 Honours Year Project
 * 
 * Implements a hash table to cache previously evaluated positions,
 * avoiding redundant calculations and improving search efficiency.
 * 
 * Features:
 * - 64-bit Zobrist hash verification
 * - Depth-preferred replacement strategy
 * - Alpha-Beta bound handling (EXACT/LOWER/UPPER)
 * - Configurable table size (power of 2)
 * 
 * Performance target: >40% cache hit rate, 2-5x search speedup
 */

#pragma once

#include <cstdint>
#include <vector>
#include <cstring>

namespace reversi::ai {

/**
 * @brief Transposition table entry
 * 
 * Stores cached search results for a position:
 * - Hash value for collision detection
 * - Evaluation score
 * - Search depth (for depth validation)
 * - Entry type (EXACT/LOWER_BOUND/UPPER_BOUND)
 * - Best move found
 * 
 * Size: 16 bytes (cache-aligned)
 */
struct TTEntry {
    uint64_t hash;        ///< 64-bit Zobrist hash (for verification)
    uint32_t hash_low;    ///< low 32-bits of hash for faster pre-checks
    int32_t score;        ///< Evaluation score (use 32-bit to avoid overflow)
    int8_t depth;         ///< Search depth
    uint8_t flag;         ///< Entry type: EXACT, LOWER_BOUND, UPPER_BOUND
    int8_t best_move;     ///< Best move found (0-63, or -1)
    
    // Padding to align to 24/32 bytes depending on platform (kept minimal)
    int8_t padding[3];
    
    /**
     * @brief Check if entry is valid (non-zero hash)
     */
    bool is_valid() const {
        return hash != 0;
    }
    
    /**
     * @brief Check if entry matches given hash
     */
    bool matches(uint64_t h) const {
        return hash == h;
    }
    
    /**
     * @brief Clear entry (set hash to 0)
     */
    void clear() {
        hash = 0;
        hash_low = 0;
    }
};

/**
 * @brief Entry type flags
 */
enum class TTFlag : uint8_t {
    EXACT = 0,          ///< Exact score (alpha < score < beta)
    LOWER_BOUND = 1,    ///< Beta cutoff (score >= beta)
    UPPER_BOUND = 2     ///< Alpha cutoff (score <= alpha)
};

/**
 * @brief Transposition table for caching search results
 * 
 * Uses a hash table to store previously evaluated positions.
 * Implements depth-preferred replacement strategy.
 * 
 * Table size: 2^size_bits entries (default: 2^20 = 1M entries, ~16 MB)
 */
class TranspositionTable {
public:
    /**
     * @brief Construct transposition table
     * @param size_bits Table size = 2^size_bits entries
     *                  Default: 20 (1M entries, ~16 MB)
     */
    explicit TranspositionTable(int size_bits = 20);
    
    /**
     * @brief Destructor
     */
    ~TranspositionTable() = default;
    
    // Non-copyable (large table)
    TranspositionTable(const TranspositionTable&) = delete;
    TranspositionTable& operator=(const TranspositionTable&) = delete;
    
    // Movable
    TranspositionTable(TranspositionTable&&) = default;
    TranspositionTable& operator=(TranspositionTable&&) = default;
    
    /**
     * @brief Query entry for given hash
     * @param hash 64-bit Zobrist hash
     * @return Pointer to entry if found and valid, nullptr otherwise
     * 
     * Checks:
     * - Entry exists (hash != 0)
     * - Hash matches (collision detection)
     */
    TTEntry* probe(uint64_t hash) const;
    
    /**
     * @brief Store entry in table
     * @param entry Entry to store
     * 
     * Replacement strategy: depth-preferred
     * - Replace if slot is empty
     * - Replace if new entry has greater depth
     * - Replace if same depth but different position (collision)
     */
    void store(const TTEntry& entry);
    
    /**
     * @brief Clear all entries
     */
    void clear();
    
    /**
     * @brief Get table size (number of entries)
     */
    size_t size() const { return table_.size(); }
    
    /**
     * @brief Get number of stored entries
     */
    size_t entries() const { return entries_; }
    
    /**
     * @brief Get cache hit rate
     * @return Hit rate (hits / (hits + misses))
     */
    double hit_rate() const {
        size_t total = hits_ + misses_;
        return total > 0 ? static_cast<double>(hits_) / total : 0.0;
    }
    
    /**
     * @brief Get statistics
     */
    struct Stats {
        size_t size;           ///< Table size
        size_t entries;         ///< Number of stored entries
        size_t hits;            ///< Cache hits
        size_t misses;          ///< Cache misses
        double hit_rate;        ///< Hit rate (0.0 to 1.0)
    };
    
    Stats get_stats() const {
        return {size(), entries_, hits_, misses_, hit_rate()};
    }
    
    /**
     * @brief Reset statistics (hits/misses)
     */
    void reset_stats() {
        hits_ = 0;
        misses_ = 0;
    }
    
    /**
     * @brief Get number of hits
     */
    size_t hits() const { return hits_; }
    
    /**
     * @brief Get number of misses
     */
    size_t misses() const { return misses_; }
    
private:
    std::vector<TTEntry> table_;  ///< Hash table
    size_t size_mask_;            ///< Mask for fast modulo (size - 1)
    size_t entries_;              ///< Number of stored entries
    mutable size_t hits_;         ///< Cache hits (mutable for const probe)
    mutable size_t misses_;       ///< Cache misses (mutable for const probe)
};

} // namespace reversi::ai

