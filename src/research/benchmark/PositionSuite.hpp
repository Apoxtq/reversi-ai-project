/*
 * PositionSuite.hpp - Test Position Suite Generator
 * COMP390 Honours Year Project - Week 10
 * 
 * Generates diverse test positions for benchmarking
 * Supports opening, midgame, and endgame positions
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#pragma once

#include "../../core/Board.hpp"
#include <vector>
#include <string>
#include <cstdint>
#include <random>

namespace reversi {
namespace research {

/**
 * @brief Test position suite generator
 * 
 * Generates diverse positions for comprehensive benchmarking:
 * - Opening positions (first 10 moves)
 * - Midgame positions (10-40 moves)
 * - Endgame positions (40+ moves)
 * - Random positions with controlled parameters
 */
class PositionSuite {
public:
    /**
     * @brief Position suite type
     */
    enum class SuiteType {
        OPENING,      ///< Opening positions (first 10 moves)
        MIDGAME,      ///< Midgame positions (10-40 moves)
        ENDGAME,      ///< Endgame positions (40+ moves)
        STANDARD_64,  ///< Standard 64-position suite (mixed)
        RANDOM        ///< Random positions
    };
    
    /**
     * @brief Generate standard position suite
     * 
     * @param type Suite type
     * @param count Number of positions to generate
     * @param seed Random seed (0 = use time)
     * @return Vector of board positions
     */
    static std::vector<core::Board> generate_suite(
        SuiteType type,
        int count = 64,
        uint32_t seed = 0
    );
    
    /**
     * @brief Generate opening positions
     * 
     * Creates positions after N random moves from starting position
     * 
     * @param count Number of positions
     * @param min_moves Minimum moves from start (default: 4)
     * @param max_moves Maximum moves from start (default: 10)
     * @param seed Random seed
     * @return Vector of opening positions
     */
    static std::vector<core::Board> generate_opening(
        int count = 20,
        int min_moves = 4,
        int max_moves = 10,
        uint32_t seed = 0
    );
    
    /**
     * @brief Generate midgame positions
     * 
     * Creates positions after 10-40 moves from starting position
     * 
     * @param count Number of positions
     * @param min_moves Minimum moves (default: 10)
     * @param max_moves Maximum moves (default: 40)
     * @param seed Random seed
     * @return Vector of midgame positions
     */
    static std::vector<core::Board> generate_midgame(
        int count = 20,
        int min_moves = 10,
        int max_moves = 40,
        uint32_t seed = 0
    );
    
    /**
     * @brief Generate endgame positions
     * 
     * Creates positions after 40+ moves from starting position
     * 
     * @param count Number of positions
     * @param min_moves Minimum moves (default: 40)
     * @param max_moves Maximum moves (default: 55)
     * @param seed Random seed
     * @return Vector of endgame positions
     */
    static std::vector<core::Board> generate_endgame(
        int count = 20,
        int min_moves = 40,
        int max_moves = 55,
        uint32_t seed = 0
    );
    
    /**
     * @brief Generate random positions
     * 
     * Creates positions by playing random moves from starting position
     * 
     * @param count Number of positions
     * @param min_moves Minimum moves from start
     * @param max_moves Maximum moves from start
     * @param seed Random seed
     * @return Vector of random positions
     */
    static std::vector<core::Board> generate_random(
        int count,
        int min_moves = 10,
        int max_moves = 50,
        uint32_t seed = 0
    );
    
    /**
     * @brief Generate standard 64-position suite
     * 
     * Mix of opening (20), midgame (24), and endgame (20) positions
     * 
     * @param seed Random seed
     * @return Vector of 64 diverse positions
     */
    static std::vector<core::Board> generate_standard_64(uint32_t seed = 0);
    
private:
    // Internal helpers implemented in PositionSuite.cpp (not exposed here to avoid
    // depending on <random> in the header).
};

} // namespace research
} // namespace reversi


     */
    static int get_random_move(const core::Board& board, std::mt19937& rng);
};

} // namespace research
} // namespace reversi


