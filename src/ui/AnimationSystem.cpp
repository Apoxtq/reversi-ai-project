/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Animation System Implementation
 */

#include "AnimationSystem.hpp"
#include <algorithm>
#include <cmath>

namespace reversi {
namespace ui {

// ==================== ScaleAnimation ====================

ScaleAnimation::ScaleAnimation(float duration, float start_scale, float end_scale)
    : duration_(duration)
    , start_scale_(start_scale)
    , end_scale_(end_scale)
{
}

bool ScaleAnimation::update(float dt) {
    elapsed_ += dt;
    return elapsed_ >= duration_;
}

float ScaleAnimation::get_progress() const {
    if (duration_ <= 0.0f) return 1.0f;
    return std::min(1.0f, elapsed_ / duration_);
}

bool ScaleAnimation::is_complete() const {
    return elapsed_ >= duration_;
}

float ScaleAnimation::get_duration() const {
    return duration_;
}

float ScaleAnimation::get_current_scale() const {
    float t = get_progress();
    // Ease-out interpolation
    t = 1.0f - (1.0f - t) * (1.0f - t);
    return start_scale_ + (end_scale_ - start_scale_) * t;
}

// ==================== FlipAnimation ====================

FlipAnimation::FlipAnimation(float duration, int position)
    : duration_(duration)
    , position_(position)
{
}

bool FlipAnimation::update(float dt) {
    elapsed_ += dt;
    return elapsed_ >= duration_;
}

float FlipAnimation::get_progress() const {
    if (duration_ <= 0.0f) return 1.0f;
    return std::min(1.0f, elapsed_ / duration_);
}

bool FlipAnimation::is_complete() const {
    return elapsed_ >= duration_;
}

float FlipAnimation::get_duration() const {
    return duration_;
}

float FlipAnimation::get_rotation() const {
    return get_progress() * 180.0f; // 0 to 180 degrees
}

bool FlipAnimation::should_switch_color() const {
    return get_progress() >= 0.5f; // At 90 degrees (halfway)
}

// ==================== AnimationSystem ====================

void AnimationSystem::update(float dt) {
    animations_.erase(
        std::remove_if(animations_.begin(), animations_.end(),
            [dt](std::unique_ptr<Animation>& anim) {
                return anim->update(dt);
            }),
        animations_.end()
    );
}

void AnimationSystem::add_animation(std::unique_ptr<Animation> animation) {
    if (animation) {
        animations_.push_back(std::move(animation));
    }
}

void AnimationSystem::clear() {
    animations_.clear();
}

Animation* AnimationSystem::get_animation_for_position(int position) const {
    for (const auto& anim : animations_) {
        if (auto* flip_anim = dynamic_cast<FlipAnimation*>(anim.get())) {
            if (flip_anim->get_position() == position) {
                return anim.get();
            }
        }
    }
    return nullptr;
}

bool AnimationSystem::has_active_animations() const {
    return !animations_.empty();
}

} // namespace ui
} // namespace reversi

