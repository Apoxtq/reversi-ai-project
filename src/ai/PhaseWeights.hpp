/*
 * PhaseWeights.hpp - Phase-aware evaluation weights for Reversi
 * COMP390 Honours Year Project - Week 4
 * 
 * Different game phases require different strategies:
 * - Opening (0-20): Maximize mobility, minimize frontier
 * - Early Midgame (21-35): Balance mobility and stability
 * - Late Midgame (36-50): Prioritize stability
 * - Endgame (51-64): Material count dominates
 */

#pragma once

#include "core/Board.hpp"
#include <cstdint>

namespace reversi::ai {

/**
 * @brief Game phase classification
 */
enum class GamePhase {
    Opening,     // 0-20 pieces: mobility-focused
    EarlyMid,    // 21-35 pieces: balanced strategy
    LateMid,     // 36-50 pieces: stability-focused
    Endgame      // 51-64 pieces: material-focused
};

/**
 * @brief Weight configuration for a specific game phase
 */
struct PhaseWeightConfig {
    int piece_count_weight;      // Material (disc count difference)
    int mobility_weight;         // Number of legal moves
    int potential_mobility_weight; // Empty squares adjacent to opponent
    int stability_weight;        // Stable disc advantage
    int frontier_penalty;        // Penalty for frontier discs
    int position_weight_scale;   // Scaling for position weight table
    int corner_bonus;            // Extra bonus for corners (beyond position weights)
};

/**
 * @brief Phase detection and weight management
 */
class PhaseWeights {
public:
    /**
     * @brief Detect current game phase
     * 
     * @param board The board to analyze
     * @return Current game phase
     */
    static GamePhase detect_phase(const reversi::core::Board& board) noexcept {
        int total = board.count_player() + board.count_opponent();
        
        if (total <= 20) return GamePhase::Opening;
        if (total <= 35) return GamePhase::EarlyMid;
        if (total <= 50) return GamePhase::LateMid;
        return GamePhase::Endgame;
    }
    
    /**
     * @brief Get weight configuration for current phase
     * 
     * @param phase The game phase
     * @return Weight configuration
     */
    static const PhaseWeightConfig& get_weights(GamePhase phase) noexcept {
        static const PhaseWeightConfig configs[4] = {
            // Opening: Mobility > Stability > Material
            {
                .piece_count_weight = 1,        // Material doesn't matter much
                .mobility_weight = 10,          // Maximize options
                .potential_mobility_weight = 5,  // Future mobility important
                .stability_weight = 3,          // Some stability value
                .frontier_penalty = -3,         // Avoid frontier discs
                .position_weight_scale = 5,     // Position weights active
                .corner_bonus = 25              // Corners very valuable
            },
            
            // Early Midgame: Balanced approach
            {
                .piece_count_weight = 2,        // Material starting to matter
                .mobility_weight = 8,           // Still important
                .potential_mobility_weight = 4, // Future planning
                .stability_weight = 5,          // Stability increasing
                .frontier_penalty = -2,         // Still avoid frontier
                .position_weight_scale = 5,     // Position weights active
                .corner_bonus = 30              // Corners critical
            },
            
            // Late Midgame: Stability focus
            {
                .piece_count_weight = 5,        // Material matters more
                .mobility_weight = 5,           // Less critical
                .potential_mobility_weight = 2, // Less forward-looking
                .stability_weight = 8,          // Stability critical
                .frontier_penalty = -1,         // Less penalty
                .position_weight_scale = 3,     // Position weights fading
                .corner_bonus = 35              // Corners essential
            },
            
            // Endgame: Material dominates
            {
                .piece_count_weight = 10,       // Material is everything
                .mobility_weight = 2,           // Minimal importance
                .potential_mobility_weight = 0, // Irrelevant
                .stability_weight = 10,         // Still valuable
                .frontier_penalty = 0,          // No longer matters
                .position_weight_scale = 1,     // Position weights minimal
                .corner_bonus = 40              // Corners for stability
            }
        };
        
        return configs[static_cast<int>(phase)];
    }
    
    /**
     * @brief Get weight configuration for current board state
     * 
     * @param board The board to analyze
     * @return Weight configuration for current phase
     */
    static const PhaseWeightConfig& get_weights(const reversi::core::Board& board) noexcept {
        return get_weights(detect_phase(board));
    }
    
    /**
     * @brief Calculate smooth interpolation between phases (advanced)
     * 
     * Instead of abrupt phase transitions, smoothly interpolate weights.
     * 
     * @param board The board
     * @return Interpolated weight configuration
     */
    static PhaseWeightConfig get_smooth_weights(const reversi::core::Board& board) noexcept {
        int total = board.count_player() + board.count_opponent();
        
        // Normalize to [0, 1] range across full game
        float progress = static_cast<float>(total) / 64.0f;
        
        // Simple linear interpolation between opening and endgame
        // Can be replaced with more sophisticated curves
        PhaseWeightConfig result;
        
        // Example: linear interpolation
        result.piece_count_weight = static_cast<int>(1.0f + progress * 9.0f);
        result.mobility_weight = static_cast<int>(10.0f - progress * 8.0f);
        result.stability_weight = static_cast<int>(3.0f + progress * 7.0f);
        result.frontier_penalty = static_cast<int>(-3.0f + progress * 3.0f);
        result.position_weight_scale = static_cast<int>(5.0f - progress * 4.0f);
        result.potential_mobility_weight = static_cast<int>(5.0f - progress * 5.0f);
        result.corner_bonus = static_cast<int>(25.0f + progress * 15.0f);
        
        return result;
    }
    
    /**
     * @brief Get phase name for debugging
     */
    static const char* phase_name(GamePhase phase) noexcept {
        switch (phase) {
            case GamePhase::Opening: return "Opening";
            case GamePhase::EarlyMid: return "EarlyMid";
            case GamePhase::LateMid: return "LateMid";
            case GamePhase::Endgame: return "Endgame";
            default: return "Unknown";
        }
    }
};

} // namespace reversi::ai

