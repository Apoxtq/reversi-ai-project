/*
 * StabilityAnalyzer.hpp - Stability analysis for Reversi
 * COMP390 Honours Year Project - Week 4
 * 
 * Analyzes piece stability:
 * - Stable pieces: Cannot be flipped under any circumstances
 * - Semi-stable: Currently safe but could become vulnerable
 * - Unstable: Can be flipped immediately
 * 
 * Also calculates frontier discs (pieces adjacent to empty squares)
 * which are strategically weak in opening/midgame.
 */

#pragma once

#include "core/Board.hpp"
#include <cstdint>

namespace reversi::ai {

/**
 * @brief Result of stability analysis
 */
struct StabilityResult {
    int stable_count;          // Player stable - opponent stable
    int semi_stable_count;     // Player semi-stable - opponent semi-stable
    int frontier_count;        // Player frontier - opponent frontier
    
    // Bitboards of stable pieces (for debugging/visualization)
    uint64_t player_stable;
    uint64_t opponent_stable;
};

/**
 * @brief Analyzer for piece stability and frontier discs
 * 
 * Stability is crucial in Reversi:
 * - Stable pieces cannot be flipped → permanent advantage
 * - Corners are always stable once captured
 * - Stability propagates from corners along edges
 * - Full stability requires line control (row/col/diagonal)
 * 
 * Frontier discs are strategically weak:
 * - Adjacent to empty squares → easy to flip
 * - Give opponent mobility
 * - Should be minimized in opening/midgame
 */
class StabilityAnalyzer {
public:
    /**
     * @brief Perform full stability analysis
     * 
     * @param board The board to analyze
     * @return Stability metrics (stable, semi-stable, frontier)
     * 
     * Algorithm:
     * 1. Identify captured corners
     * 2. Propagate stability from corners along edges
     * 3. Calculate interior stability (requires full lines)
     * 4. Count frontier discs
     * 
     * Performance: < 100 cycles per call
     */
    static StabilityResult analyze(const reversi::core::Board& board) noexcept;
    
    /**
     * @brief Fast corner stability analysis (simplified)
     * 
     * Only analyzes corner-based stability, ignoring interior.
     * Much faster but less accurate.
     * 
     * Use for deep search nodes where speed matters.
     * 
     * Performance: < 50 cycles per call
     */
    static int calculate_corner_stability(const reversi::core::Board& board) noexcept;
    
    /**
     * @brief Calculate frontier disc penalty
     * 
     * Frontier disc: piece adjacent to at least one empty square
     * 
     * @param board The board to analyze
     * @return Player frontier count - opponent frontier count
     * 
     * Performance: ~30 cycles (8 shift operations)
     */
    static int calculate_frontier(const reversi::core::Board& board) noexcept;
    
private:
    // Corner positions (bitboard masks)
    static constexpr uint64_t CORNER_A1 = 0x0000000000000001ULL;  // Bottom-left
    static constexpr uint64_t CORNER_H1 = 0x0000000000000080ULL;  // Bottom-right
    static constexpr uint64_t CORNER_A8 = 0x0100000000000000ULL;  // Top-left
    static constexpr uint64_t CORNER_H8 = 0x8000000000000000ULL;  // Top-right
    static constexpr uint64_t ALL_CORNERS = CORNER_A1 | CORNER_H1 | CORNER_A8 | CORNER_H8;
    
    // Edge masks (rows/cols 0 and 7)
    static constexpr uint64_t EDGE_ROW_0 = 0x00000000000000FFULL;
    static constexpr uint64_t EDGE_ROW_7 = 0xFF00000000000000ULL;
    static constexpr uint64_t EDGE_COL_0 = 0x0101010101010101ULL;
    static constexpr uint64_t EDGE_COL_7 = 0x8080808080808080ULL;
    static constexpr uint64_t ALL_EDGES = EDGE_ROW_0 | EDGE_ROW_7 | EDGE_COL_0 | EDGE_COL_7;
    
    /**
     * @brief Check if a specific corner is captured by a player
     * 
     * @param pieces Player's bitboard
     * @param corner_mask Corner position mask
     * @return true if corner is occupied by player
     */
    static inline bool is_corner_captured(uint64_t pieces, uint64_t corner_mask) noexcept {
        return (pieces & corner_mask) != 0;
    }
    
    /**
     * @brief Count stable pieces in a corner wedge
     * 
     * Once a corner is captured, adjacent pieces along edges become stable
     * if there are no gaps between them and the corner.
     * 
     * Example (corner A1 captured):
     * X . . . . . . .    Stable: X, XX on row 0, X on col 0
     * X . . . . . . .    
     * X . . . . . . .    
     * . . . . . . . .
     * 
     * @param board The board
     * @param corner Corner position (0=A1, 7=H1, 56=A8, 63=H8)
     * @return Number of stable pieces from this corner
     */
    static int count_stable_from_corner(
        const reversi::core::Board& board,
        int corner,
        uint64_t player_pieces) noexcept;
    
    /**
     * @brief Propagate stability along an edge
     * 
     * Starting from a stable corner, mark adjacent edge pieces as stable
     * until we hit a gap or opponent piece.
     * 
     * @param pieces Player's pieces
     * @param stable Current stable bitboard (updated)
     * @param start Start position
     * @param direction Direction to propagate (-1, +1, -8, +8)
     * @param limit How many squares to check
     */
    static void propagate_edge_stability(
        uint64_t pieces,
        uint64_t& stable,
        int start,
        int direction,
        int limit) noexcept;
    
    /**
     * @brief Calculate interior stability (pieces in full lines)
     * 
     * A piece is fully stable if it's in complete lines (row/col/diag)
     * with stable endpoints.
     * 
     * This is the most accurate but also slowest method.
     * 
     * @param board The board
     * @param edge_stable Already-stable edge pieces
     * @return Bitboard of additional stable interior pieces
     */
    static uint64_t calculate_interior_stability(
        const reversi::core::Board& board,
        uint64_t edge_stable) noexcept;
    
    /**
     * @brief Count frontier discs using bitboard shifts
     * 
     * Algorithm:
     * 1. Get all empty squares
     * 2. Shift empty in all 8 directions
     * 3. Count pieces adjacent to shifted empty squares
     * 
     * @param pieces Pieces to check
     * @param empty Empty squares bitboard
     * @return Number of pieces adjacent to empty
     */
    static inline int count_adjacent_to_empty(uint64_t pieces, uint64_t empty) noexcept;
    
    // Bitboard shift helpers (from Board.hpp direction logic)
    static inline uint64_t shift_N(uint64_t bb) noexcept { return bb << 8; }
    static inline uint64_t shift_S(uint64_t bb) noexcept { return bb >> 8; }
    static inline uint64_t shift_E(uint64_t bb) noexcept { return (bb << 1) & 0xFEFEFEFEFEFEFEFEULL; }
    static inline uint64_t shift_W(uint64_t bb) noexcept { return (bb >> 1) & 0x7F7F7F7F7F7F7F7FULL; }
    static inline uint64_t shift_NE(uint64_t bb) noexcept { return (bb << 9) & 0xFEFEFEFEFEFEFEFEULL; }
    static inline uint64_t shift_NW(uint64_t bb) noexcept { return (bb << 7) & 0x7F7F7F7F7F7F7F7FULL; }
    static inline uint64_t shift_SE(uint64_t bb) noexcept { return (bb >> 7) & 0xFEFEFEFEFEFEFEFEULL; }
    static inline uint64_t shift_SW(uint64_t bb) noexcept { return (bb >> 9) & 0x7F7F7F7F7F7F7F7FULL; }
};

} // namespace reversi::ai

