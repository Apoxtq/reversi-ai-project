/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Menu System Implementation
 */

#include "MenuSystem.hpp"
#include "UIStyle.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace reversi {
namespace ui {

// ==================== MainMenuState Implementation ====================

MainMenuState::MainMenuState() {
    create_buttons();
}

void MainMenuState::create_buttons() {
    // Button layout: 5 buttons vertically centered
    float button_width = 300.0f;
    float button_height = 60.0f;
    float button_spacing = 20.0f;
    float start_y = (UIStyle::WINDOW_HEIGHT - (5 * button_height + 4 * button_spacing)) / 2.0f;
    float center_x = (UIStyle::WINDOW_WIDTH - button_width) / 2.0f;
    
    buttons_.clear();
    
    // Button 1: Play vs AI
    auto btn1 = std::make_unique<Button>(
        "Play vs AI",
        sf::Vector2f(center_x, start_y),
        sf::Vector2f(button_width, button_height),
        Button::Style::PRIMARY
    );
    btn1->set_callback([this]() { selection_ = Selection::VS_AI; });
    buttons_.push_back(std::move(btn1));
    
    // Button 2: Play Local 2P
    auto btn2 = std::make_unique<Button>(
        "Local 2-Player",
        sf::Vector2f(center_x, start_y + button_height + button_spacing),
        sf::Vector2f(button_width, button_height),
        Button::Style::PRIMARY
    );
    btn2->set_callback([this]() { selection_ = Selection::LOCAL_2P; });
    buttons_.push_back(std::move(btn2));
    
    // Button 3: Online (Week 8) - Now enabled!
    auto btn3 = std::make_unique<Button>(
        "Online",
        sf::Vector2f(center_x, start_y + 2 * (button_height + button_spacing)),
        sf::Vector2f(button_width, button_height),
        Button::Style::PRIMARY
    );
    btn3->set_callback([this]() { selection_ = Selection::ONLINE; });
    buttons_.push_back(std::move(btn3));
    
    // Button 4: Benchmark Suite
    auto btn4 = std::make_unique<Button>(
        "Benchmark Suite",
        sf::Vector2f(center_x, start_y + 3 * (button_height + button_spacing)),
        sf::Vector2f(button_width, button_height),
        Button::Style::SECONDARY
    );
    btn4->set_callback([this]() { selection_ = Selection::BENCHMARK; });
    buttons_.push_back(std::move(btn4));
    
    // Button 5: Exit
    auto btn5 = std::make_unique<Button>(
        "Exit",
        sf::Vector2f(center_x, start_y + 4 * (button_height + button_spacing)),
        sf::Vector2f(button_width, button_height),
        Button::Style::SECONDARY
    );
    btn5->set_callback([this]() { selection_ = Selection::EXIT; });
    buttons_.push_back(std::move(btn5));
}

void MainMenuState::update(float dt) {
    for (auto& button : buttons_) {
        if (button) {
            button->update(dt);
        }
    }
}

void MainMenuState::render(sf::RenderTarget& target) {
    // Draw background
    sf::RectangleShape background(sf::Vector2f(UIStyle::WINDOW_WIDTH, UIStyle::WINDOW_HEIGHT));
    background.setFillColor(UIStyle::BACKGROUND_L1);
    target.draw(background);
    
    // Draw title (placeholder - would need font)
    // sf::Text title("Reversi AI Platform", font, UIStyle::FONT_H1);
    // title.setPosition(...);
    // target.draw(title);
    
    // Draw buttons
    for (const auto& button : buttons_) {
        if (button && button->is_visible()) {
            target.draw(*button);
        }
    }
}

bool MainMenuState::handle_event(const sf::Event& event) {
    // Forward events to buttons
    for (auto& button : buttons_) {
        if (button && button->handle_event(event)) {
            return true;
        }
    }
    return false;
}

std::unique_ptr<MenuState> MainMenuState::get_next_state() {
    if (selection_ != Selection::NONE) {
        // State transition handled by MenuSystem
        return nullptr;
    }
    return nullptr;
}

bool MainMenuState::is_finished() const {
    return selection_ != Selection::NONE;
}

// ==================== MenuSystem Implementation ====================

MenuSystem::MenuSystem() {
    initialize();
}

void MenuSystem::initialize() {
    current_state_ = std::make_unique<MainMenuState>();
}

void MenuSystem::update(float dt) {
    if (current_state_) {
        current_state_->update(dt);
        transition_to_next_state();
    }
}

void MenuSystem::render(sf::RenderTarget& target) {
    if (current_state_) {
        current_state_->render(target);
    }
}

void MenuSystem::handle_event(const sf::Event& event) {
    if (current_state_) {
        current_state_->handle_event(event);
    }
}

MainMenuState::Selection MenuSystem::get_selection() const {
    if (auto* main_menu = dynamic_cast<MainMenuState*>(current_state_.get())) {
        return main_menu->get_selection();
    }
    return MainMenuState::Selection::NONE;
}

void MenuSystem::transition_to_next_state() {
    if (current_state_ && current_state_->is_finished()) {
        auto next_state = current_state_->get_next_state();
        if (next_state) {
            current_state_ = std::move(next_state);
        }
    }
}

} // namespace ui
} // namespace reversi

