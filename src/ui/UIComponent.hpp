/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * UI Component Base Class (Factory Pattern)
 * Base class for all UI components (buttons, panels, dialogs)
 */

#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <functional>
#include <string>
#include <vector>

namespace reversi {
namespace ui {

/**
 * @brief Base class for all UI components
 * 
 * Implements Factory Pattern - all UI components inherit from this base.
 * Provides unified interface for rendering, event handling, and updates.
 * 
 * Components:
 * - Button
 * - Panel
 * - Dialog
 * - TextInput
 * - etc.
 */
class UIComponent : public sf::Drawable {
public:
    virtual ~UIComponent() = default;
    
    /**
     * @brief Draw the component
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    
    /**
     * @brief Handle input event
     * @param event SFML event
     * @return true if event was handled
     */
    virtual bool handle_event(const sf::Event& event) = 0;
    
    /**
     * @brief Update component (animation, state changes)
     * @param dt Delta time in seconds
     */
    virtual void update(float dt) = 0;
    
    /**
     * @brief Check if component contains point
     * @param point Screen coordinates
     * @return true if point is inside component
     */
    virtual bool contains(const sf::Vector2f& point) const = 0;
    
    /**
     * @brief Set component position
     */
    virtual void set_position(const sf::Vector2f& position) = 0;
    
    /**
     * @brief Get component position
     */
    virtual sf::Vector2f get_position() const = 0;
    
    /**
     * @brief Set component visibility
     */
    virtual void set_visible(bool visible) { visible_ = visible; }
    
    /**
     * @brief Check if component is visible
     */
    virtual bool is_visible() const { return visible_; }
    
    /**
     * @brief Set component enabled state
     */
    virtual void set_enabled(bool enabled) { enabled_ = enabled; }
    
    /**
     * @brief Check if component is enabled
     */
    virtual bool is_enabled() const { return enabled_; }
    
protected:
    bool visible_ = true;
    bool enabled_ = true;
};

/**
 * @brief Button component
 * 
 * Reusable button with hover, press, and disabled states.
 * Follows UI/UX Design Specification button specifications.
 */
class Button : public UIComponent {
public:
    /**
     * @brief Button style
     */
    enum class Style {
        PRIMARY,    // Green accent (#4a9d5f)
        SECONDARY,  // Gray (#3a3a3a)
        ICON        // Icon button (transparent background)
    };
    
    /**
     * @brief Construct button
     * @param text Button text
     * @param position Button position
     * @param size Button size
     * @param style Button style
     */
    Button(const std::string& text, 
           const sf::Vector2f& position,
           const sf::Vector2f& size,
           Style style = Style::PRIMARY);
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool handle_event(const sf::Event& event) override;
    void update(float dt) override;
    bool contains(const sf::Vector2f& point) const override;
    void set_position(const sf::Vector2f& position) override;
    sf::Vector2f get_position() const override;
    
    /**
     * @brief Set button text
     */
    void set_text(const std::string& text);
    
    /**
     * @brief Get button text
     */
    std::string get_text() const { return text_; }
    
    /**
     * @brief Set callback function (called on click)
     */
    void set_callback(std::function<void()> callback) { callback_ = callback; }
    
    /**
     * @brief Check if button was clicked
     */
    bool was_clicked() const { return clicked_; }
    
    /**
     * @brief Reset click state
     */
    void reset_click() { clicked_ = false; }
    
private:
    std::string text_;
    sf::Vector2f position_;
    sf::Vector2f size_;
    Style style_;
    
    bool hovered_ = false;
    bool pressed_ = false;
    bool clicked_ = false;
    
    std::function<void()> callback_;
    
    // Visual state
    float hover_alpha_ = 0.0f;  // For smooth hover transition
};

/**
 * @brief Panel component
 * 
 * Container for other UI components.
 * Used for right panel, dialogs, etc.
 */
class Panel : public UIComponent {
public:
    /**
     * @brief Construct panel
     * @param position Panel position
     * @param size Panel size
     */
    Panel(const sf::Vector2f& position, const sf::Vector2f& size);
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool handle_event(const sf::Event& event) override;
    void update(float dt) override;
    bool contains(const sf::Vector2f& point) const override;
    void set_position(const sf::Vector2f& position) override;
    sf::Vector2f get_position() const override;
    
    /**
     * @brief Add child component
     */
    void add_component(std::unique_ptr<UIComponent> component);
    
    /**
     * @brief Clear all components
     */
    void clear();
    
private:
    sf::Vector2f position_;
    sf::Vector2f size_;
    std::vector<std::unique_ptr<UIComponent>> children_;
};

} // namespace ui
} // namespace reversi

