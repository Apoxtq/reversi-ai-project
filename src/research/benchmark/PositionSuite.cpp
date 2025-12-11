/*
 * PositionSuite.cpp - Test Position Suite Generator Implementation
 * COMP390 Honours Year Project - Week 10
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 */

#include "PositionSuite.hpp"
#include <random>
#include <chrono>
#include <algorithm>

namespace reversi {
namespace research {

std::vector<core::Board> PositionSuite::generate_suite(
    SuiteType type,
    int count,
    uint32_t seed
) {
    switch (type) {
        case SuiteType::OPENING:
            return generate_opening(count, 4, 10, seed);
        case SuiteType::MIDGAME:
            return generate_midgame(count, 10, 40, seed);
        case SuiteType::ENDGAME:
            return generate_endgame(count, 40, 55, seed);
        case SuiteType::STANDARD_64:
            return generate_standard_64(seed);
        case SuiteType::RANDOM:
            return generate_random(count, 10, 50, seed);
        default:
            return generate_standard_64(seed);
    }
}

std::vector<core::Board> PositionSuite::generate_opening(
    int count,
    int min_moves,
    int max_moves,
    uint32_t seed
) {
    std::vector<core::Board> positions;
    std::mt19937 rng(seed == 0 ? 
                     std::chrono::steady_clock::now().time_since_epoch().count() : 
                     seed);
    std::uniform_int_distribution<int> move_dist(min_moves, max_moves);
    
    for (int i = 0; i < count; ++i) {
        core::Board board; // Standard starting position
        int num_moves = move_dist(rng);
        core::Board position = play_random_moves(board, num_moves, rng);
        positions.push_back(position);
    }
    
    return positions;
}

std::vector<core::Board> PositionSuite::generate_midgame(
    int count,
    int min_moves,
    int max_moves,
    uint32_t seed
) {
    std::vector<core::Board> positions;
    std::mt19937 rng(seed == 0 ? 
                     std::chrono::steady_clock::now().time_since_epoch().count() : 
                     seed);
    std::uniform_int_distribution<int> move_dist(min_moves, max_moves);
    
    for (int i = 0; i < count; ++i) {
        core::Board board; // Standard starting position
        int num_moves = move_dist(rng);
        core::Board position = play_random_moves(board, num_moves, rng);
        positions.push_back(position);
    }
    
    return positions;
}

std::vector<core::Board> PositionSuite::generate_endgame(
    int count,
    int min_moves,
    int max_moves,
    uint32_t seed
) {
    std::vector<core::Board> positions;
    std::mt19937 rng(seed == 0 ? 
                     std::chrono::steady_clock::now().time_since_epoch().count() : 
                     seed);
    std::uniform_int_distribution<int> move_dist(min_moves, max_moves);
    
    for (int i = 0; i < count; ++i) {
        core::Board board; // Standard starting position
        int num_moves = move_dist(rng);
        core::Board position = play_random_moves(board, num_moves, rng);
        
        // Ensure we don't go past terminal state
        if (!position.is_terminal()) {
            positions.push_back(position);
        } else {
            // If terminal, try with fewer moves
            i--; // Retry this position
        }
    }
    
    return positions;
}

std::vector<core::Board> PositionSuite::generate_random(
    int count,
    int min_moves,
    int max_moves,
    uint32_t seed
) {
    std::vector<core::Board> positions;
    std::mt19937 rng(seed == 0 ? 
                     std::chrono::steady_clock::now().time_since_epoch().count() : 
                     seed);
    std::uniform_int_distribution<int> move_dist(min_moves, max_moves);
    
    for (int i = 0; i < count; ++i) {
        core::Board board; // Standard starting position
        int num_moves = move_dist(rng);
        core::Board position = play_random_moves(board, num_moves, rng);
        
        if (!position.is_terminal()) {
            positions.push_back(position);
        } else {
            i--; // Retry
        }
    }
    
    return positions;
}

std::vector<core::Board> PositionSuite::generate_standard_64(uint32_t seed) {
    std::vector<core::Board> positions;
    
    // Generate 20 opening positions
    auto opening = generate_opening(20, 4, 10, seed);
    positions.insert(positions.end(), opening.begin(), opening.end());
    
    // Generate 24 midgame positions
    auto midgame = generate_midgame(24, 10, 40, seed + 1000);
    positions.insert(positions.end(), midgame.begin(), midgame.end());
    
    // Generate 20 endgame positions
    auto endgame = generate_endgame(20, 40, 55, seed + 2000);
    positions.insert(positions.end(), endgame.begin(), endgame.end());
    
    return positions;
}

core::Board PositionSuite::play_random_moves(
    const core::Board& board,
    int num_moves,
    std::mt19937& rng
) {
    core::Board current = board;
    int moves_played = 0;
    
    while (moves_played < num_moves && !current.is_terminal()) {
        int move = get_random_move(current, rng);
        
        if (move >= 0) {
            current.make_move(move);
            moves_played++;
        } else {
            // No legal moves, pass
            current.pass();
            moves_played++;
        }
    }
    
    return current;
}

int PositionSuite::get_random_move(const core::Board& board, std::mt19937& rng) {
    std::vector<int> legal_moves;
    board.get_legal_moves(legal_moves);
    
    if (legal_moves.empty()) {
        return -1; // Pass
    }
    
    std::uniform_int_distribution<size_t> dist(0, legal_moves.size() - 1);
    return legal_moves[dist(rng)];
}

} // namespace research
} // namespace reversi

