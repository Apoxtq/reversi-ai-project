/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Animation System
 * Manages animations for disc placement and flipping
 */

#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <memory>
#include <functional>

namespace reversi {
namespace ui {

/**
 * @brief Animation base class
 */
class Animation {
public:
    virtual ~Animation() = default;
    
    /**
     * @brief Update animation
     * @param dt Delta time
     * @return true if animation is complete
     */
    virtual bool update(float dt) = 0;
    
    /**
     * @brief Get animation progress (0.0 to 1.0)
     */
    virtual float get_progress() const = 0;
    
    /**
     * @brief Check if animation is complete
     */
    virtual bool is_complete() const = 0;
    
    /**
     * @brief Get animation duration
     */
    virtual float get_duration() const = 0;
};

/**
 * @brief Scale animation (for disc placement)
 */
class ScaleAnimation : public Animation {
public:
    ScaleAnimation(float duration, float start_scale = 0.0f, float end_scale = 1.0f);
    
    bool update(float dt) override;
    float get_progress() const override;
    bool is_complete() const override;
    float get_duration() const override;
    
    float get_current_scale() const;
    
private:
    float duration_;
    float elapsed_ = 0.0f;
    float start_scale_;
    float end_scale_;
};

/**
 * @brief Flip animation (for disc flipping)
 */
class FlipAnimation : public Animation {
public:
    FlipAnimation(float duration, int position);
    
    bool update(float dt) override;
    float get_progress() const override;
    bool is_complete() const override;
    float get_duration() const override;
    
    int get_position() const { return position_; }
    float get_rotation() const;
    bool should_switch_color() const; // At 90 degrees
    
private:
    float duration_;
    float elapsed_ = 0.0f;
    int position_;
};

/**
 * @brief Animation system manager
 */
class AnimationSystem {
public:
    /**
     * @brief Update all animations
     * @param dt Delta time
     */
    void update(float dt);
    
    /**
     * @brief Add animation
     */
    void add_animation(std::unique_ptr<Animation> animation);
    
    /**
     * @brief Clear all animations
     */
    void clear();
    
    /**
     * @brief Get animation for position (if exists)
     */
    Animation* get_animation_for_position(int position) const;
    
    /**
     * @brief Check if any animations are active
     */
    bool has_active_animations() const;
    
private:
    std::vector<std::unique_ptr<Animation>> animations_;
};

} // namespace ui
} // namespace reversi

