/*
 * Reversi AI Algorithm Benchmarking and Optimisation Research
 * COMP390 Honours Year Project (2025–26)
 * 
 * Author: Tianqixing
 * Student ID: 201821852
 * 
 * Main UI Entry Point
 * Week 7: SFML UI Application
 */

#include "ui/GameUI.hpp"
#include "ai/MinimaxEngine.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

using namespace reversi::ui;
using namespace reversi::ai;

int main() {
    // Create window
    sf::RenderWindow window(
        sf::VideoMode(UIStyle::WINDOW_WIDTH, UIStyle::WINDOW_HEIGHT),
        "Reversi AI Platform",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    
    // Create GameUI
    GameUI game_ui(window);
    game_ui.initialize();
    
    // Run main loop
    game_ui.run();
    
    return 0;
}

