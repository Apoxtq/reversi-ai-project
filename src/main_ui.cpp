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
    try {
        std::cout << "Creating window..." << std::endl;
        // Create window
        sf::RenderWindow window(
            sf::VideoMode(UIStyle::WINDOW_WIDTH, UIStyle::WINDOW_HEIGHT),
            "Reversi AI Platform",
            sf::Style::Titlebar | sf::Style::Close
        );
        window.setFramerateLimit(60);
        window.setVerticalSyncEnabled(true);
        
        std::cout << "Window created successfully." << std::endl;
        std::cout << "Creating GameUI..." << std::endl;
        
        // Create GameUI
        GameUI game_ui(window);
        
        std::cout << "Initializing GameUI..." << std::endl;
        game_ui.initialize();
        
        std::cout << "GameUI initialized. Starting main loop..." << std::endl;
        
        // Run main loop
        game_ui.run();
        
        std::cout << "Main loop exited normally." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception caught!" << std::endl;
        return 1;
    }
}

