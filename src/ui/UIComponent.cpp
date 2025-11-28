/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * UI Component Implementation
 */

#include "UIComponent.hpp"
#include "UIStyle.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <algorithm>
#include <cmath>

namespace reversi {
namespace ui {

// ==================== Button Implementation ====================

Button::Button(const std::string& text, 
               const sf::Vector2f& position,
               const sf::Vector2f& size,
               Style style)
    : text_(text)
    , position_(position)
    , size_(size)
    , style_(style)
{
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!visible_) return;
    
    // Draw button background
    sf::RectangleShape background(size_);
    background.setPosition(position_);
    
    // Set color based on style and state
    if (!enabled_) {
        background.setFillColor(UIStyle::TEXT_DISABLED);
    } else if (pressed_) {
        background.setFillColor(UIStyle::ACCENT_GREEN);
    } else if (hovered_) {
        if (style_ == Style::PRIMARY) {
            background.setFillColor(UIStyle::ACCENT_GREEN);
        } else {
            background.setFillColor(UIStyle::BACKGROUND_L2);
        }
    } else {
        if (style_ == Style::PRIMARY) {
            background.setFillColor(UIStyle::ACCENT_GREEN);
        } else if (style_ == Style::SECONDARY) {
            background.setFillColor(UIStyle::BACKGROUND_L2);
        } else {
            background.setFillColor(sf::Color::Transparent);
        }
    }
    
    // Add border radius effect (simplified - SFML doesn't support rounded rectangles natively)
    background.setOutlineThickness(1.0f);
    background.setOutlineColor(UIStyle::BOARD_BORDER);
    
    target.draw(background, states);
    
    // Draw text (placeholder - would need font loading)
    // sf::Text text_obj(text_, font, UIStyle::FONT_BUTTON);
    // text_obj.setPosition(position_ + sf::Vector2f(SPACING_MD, SPACING_MD));
    // text_obj.setFillColor(enabled_ ? UIStyle::TEXT_PRIMARY : UIStyle::TEXT_DISABLED);
    // target.draw(text_obj, states);
}

bool Button::handle_event(const sf::Event& event) {
    if (!enabled_ || !visible_) return false;
    
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mouse_pos(static_cast<float>(event.mouseMove.x),
                              static_cast<float>(event.mouseMove.y));
        hovered_ = contains(mouse_pos);
        return hovered_;
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse_pos(static_cast<float>(event.mouseButton.x),
                                  static_cast<float>(event.mouseButton.y));
            if (contains(mouse_pos)) {
                pressed_ = true;
                return true;
            }
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (pressed_) {
                sf::Vector2f mouse_pos(static_cast<float>(event.mouseButton.x),
                                      static_cast<float>(event.mouseButton.y));
                if (contains(mouse_pos)) {
                    clicked_ = true;
                    if (callback_) {
                        callback_();
                    }
                }
                pressed_ = false;
                return true;
            }
        }
    }
    
    return false;
}

void Button::update(float dt) {
    // Smooth hover transition
    if (hovered_ && enabled_) {
        hover_alpha_ = std::min(1.0f, hover_alpha_ + dt * 5.0f);
    } else {
        hover_alpha_ = std::max(0.0f, hover_alpha_ - dt * 5.0f);
    }
}

bool Button::contains(const sf::Vector2f& point) const {
    return point.x >= position_.x && point.x <= position_.x + size_.x &&
           point.y >= position_.y && point.y <= position_.y + size_.y;
}

void Button::set_position(const sf::Vector2f& position) {
    position_ = position;
}

sf::Vector2f Button::get_position() const {
    return position_;
}

void Button::set_text(const std::string& text) {
    text_ = text;
}

// ==================== Panel Implementation ====================

Panel::Panel(const sf::Vector2f& position, const sf::Vector2f& size)
    : position_(position)
    , size_(size)
{
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!visible_) return;
    
    // Draw panel background
    sf::RectangleShape background(size_);
    background.setPosition(position_);
    background.setFillColor(UIStyle::BACKGROUND_L2);
    background.setOutlineThickness(1.0f);
    background.setOutlineColor(UIStyle::BOARD_BORDER);
    
    target.draw(background, states);
    
    // Draw children
    for (const auto& child : children_) {
        if (child && child->is_visible()) {
            target.draw(*child, states);
        }
    }
}

bool Panel::handle_event(const sf::Event& event) {
    if (!enabled_ || !visible_) return false;
    
    // Forward events to children (in reverse order for z-index)
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if (*it && (*it)->handle_event(event)) {
            return true;
        }
    }
    
    return false;
}

void Panel::update(float dt) {
    for (auto& child : children_) {
        if (child) {
            child->update(dt);
        }
    }
}

bool Panel::contains(const sf::Vector2f& point) const {
    return point.x >= position_.x && point.x <= position_.x + size_.x &&
           point.y >= position_.y && point.y <= position_.y + size_.y;
}

void Panel::set_position(const sf::Vector2f& position) {
    sf::Vector2f offset = position - position_;
    position_ = position;
    
    // Update children positions relative to panel
    for (auto& child : children_) {
        if (child) {
            child->set_position(child->get_position() + offset);
        }
    }
}

sf::Vector2f Panel::get_position() const {
    return position_;
}

void Panel::add_component(std::unique_ptr<UIComponent> component) {
    if (component) {
        children_.push_back(std::move(component));
    }
}

void Panel::clear() {
    children_.clear();
}

} // namespace ui
} // namespace reversi

