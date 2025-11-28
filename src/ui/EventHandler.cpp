/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Event Handler Implementation
 */

#include "EventHandler.hpp"
#include "BoardRenderer.hpp"

namespace reversi {
namespace ui {

EventHandler::EventHandler() {
}

std::unique_ptr<GameEvent> EventHandler::process_event(const sf::Event& event,
                                                         BoardRenderer* board_renderer) {
    if (!enabled_) return nullptr;
    
    switch (event.type) {
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                return handle_mouse_click(event.mouseButton, board_renderer);
            }
            break;
            
        case sf::Event::KeyPressed:
            return handle_keyboard(event.key);
            
        case sf::Event::MouseMoved:
            last_mouse_pos_ = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            break;
            
        default:
            break;
    }
    
    return nullptr;
}

std::unique_ptr<GameEvent> EventHandler::handle_mouse_click(
    const sf::Event::MouseButtonEvent& event,
    BoardRenderer* board_renderer) {
    
    if (!board_renderer) return nullptr;
    
    sf::Vector2f mouse_pos(static_cast<float>(event.x), static_cast<float>(event.y));
    int board_pos = board_renderer->screen_to_board(mouse_pos);
    
    if (board_pos >= 0 && board_pos < 64) {
        core::Move move(board_pos);
        auto game_event = std::make_unique<GameEvent>(GameEventType::MOVE_SELECTED, move);
        
        if (event_callback_) {
            event_callback_(*game_event);
        }
        
        return game_event;
    }
    
    return nullptr;
}

std::unique_ptr<GameEvent> EventHandler::handle_keyboard(const sf::Event::KeyEvent& event) {
    switch (event.code) {
        case sf::Keyboard::Escape:
            return std::make_unique<GameEvent>(GameEventType::MENU_REQUESTED);
            
        case sf::Keyboard::U:
            if (event.control) { // Ctrl+U for undo
                return std::make_unique<GameEvent>(GameEventType::UNDO_REQUESTED);
            }
            break;
            
        case sf::Keyboard::P:
            if (event.control) { // Ctrl+P for pause
                return std::make_unique<GameEvent>(GameEventType::PAUSE_REQUESTED);
            }
            break;
            
        default:
            break;
    }
    
    return nullptr;
}

} // namespace ui
} // namespace reversi

