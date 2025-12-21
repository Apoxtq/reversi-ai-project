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

// 在包含任何头文件之前添加 Windows API
#ifdef _WIN32
    // Ensure winsock2 is included before windows.h to avoid winsock/winsock2 conflicts
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#endif

#include <iostream>

// 在包含任何头文件之前添加诊断函数，测试静态初始化
void diagnostic_output(const char* msg) {
    // 使用 Windows API 直接输出，避免 C++ 流的问题
    #ifdef _WIN32
    OutputDebugStringA(msg);
    OutputDebugStringA("\n");
    #endif
    // 同时尝试标准输出
    try {
        std::cout << msg << std::endl;
        std::cout.flush();
    } catch (...) {
        // 忽略异常
    }
}

// 全局对象，测试静态初始化
struct StaticInitTest {
    StaticInitTest() {
        diagnostic_output("[STATIC INIT] StaticInitTest constructor called");
    }
} static_init_test;

// Include SFML first to ensure proper initialization
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>

// Include our headers after SFML
#include "ui/GameUI.hpp"

// Don't use using namespace at global scope - it can cause static initialization issues
// Instead, use explicit namespace qualification
// Removed: using namespace reversi::ui;
// Removed: using namespace reversi::ai;

int main() {
    diagnostic_output("[MAIN] Entering main()");
    
    // Immediate output to verify program starts
    // Use unbuffered output to ensure messages appear even if program crashes
    std::cout.setf(std::ios::unitbuf);
    std::cerr.setf(std::ios::unitbuf);
    
    diagnostic_output("[MAIN] Output streams configured");
    
    std::cout << "Program starting..." << std::endl;
    std::cout.flush();
    std::cerr.flush();
    
    diagnostic_output("[MAIN] Initial output sent");
    
    // Don't test UIStyle constants here - they might cause static initialization issues
    // We'll access them directly when needed
    
    try {
        std::cout << "========================================" << std::endl;
        std::cout << "  Reversi AI Platform - Starting..." << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout.flush();
        
        std::cout << "[1/4] Creating SFML window..." << std::endl;
        std::cout.flush();
        
        // Create window with explicit error handling
        sf::RenderWindow window;
        std::cout << "  [Step 1.1] RenderWindow object created" << std::endl;
        std::cout.flush();
        
        try {
            std::cout << "  [Step 1.2] Calling window.create()..." << std::endl;
            std::cout.flush();
            
            window.create(
                sf::VideoMode(reversi::ui::UIStyle::WINDOW_WIDTH, reversi::ui::UIStyle::WINDOW_HEIGHT),
                "Reversi AI Platform",
                sf::Style::Titlebar | sf::Style::Close
            );
            
            std::cout << "  [Step 1.3] window.create() completed" << std::endl;
            std::cout.flush();
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to create window: " << e.what() << std::endl;
            std::cerr << "Exception type: " << typeid(e).name() << std::endl;
            std::cerr << "Please check SFML installation and graphics drivers." << std::endl;
            std::cerr.flush();
            return 1;
        } catch (...) {
            std::cerr << "ERROR: Unknown exception during window creation!" << std::endl;
            std::cerr.flush();
            return 1;
        }
        
        std::cout << "  [Step 1.4] Checking if window is open..." << std::endl;
        std::cout.flush();
        
        if (!window.isOpen()) {
            std::cerr << "ERROR: Window was not created successfully." << std::endl;
            std::cerr.flush();
            return 1;
        }
        
        window.setFramerateLimit(60);
        window.setVerticalSyncEnabled(true);
        std::cout << "  ✓ Window created: " << reversi::ui::UIStyle::WINDOW_WIDTH 
                  << "x" << reversi::ui::UIStyle::WINDOW_HEIGHT << std::endl;
        
        std::cout << "[2/4] Creating GameUI controller..." << std::endl;
        std::cout.flush();
        
        // Create GameUI with error handling
        reversi::ui::GameUI* game_ui_ptr = nullptr;
        try {
            game_ui_ptr = new reversi::ui::GameUI(window);
            std::cout << "  ✓ GameUI controller created" << std::endl;
            std::cout.flush();
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to create GameUI: " << e.what() << std::endl;
            std::cerr.flush();
            window.close();
            return 1;
        } catch (...) {
            std::cerr << "ERROR: Unknown exception during GameUI creation!" << std::endl;
            std::cerr.flush();
            window.close();
            return 1;
        }
        
        reversi::ui::GameUI& game_ui = *game_ui_ptr;
        
        std::cout << "[3/4] Initializing UI components..." << std::endl;
        try {
            std::cout << "  [Step 1] Starting GameUI::initialize()..." << std::endl;
            game_ui.initialize();
            std::cout << "  ✓ UI components initialized" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to initialize UI: " << e.what() << std::endl;
            std::cerr << "Exception type: " << typeid(e).name() << std::endl;
            std::cerr << "Some features may not work correctly." << std::endl;
            // Continue anyway - some components may still work
        } catch (...) {
            std::cerr << "ERROR: Unknown exception during UI initialization!" << std::endl;
            std::cerr << "This may indicate a memory corruption or system issue." << std::endl;
        }
        
        std::cout << "[4/4] Starting main game loop..." << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "  Game is ready! Close window to exit." << std::endl;
        std::cout << "========================================" << std::endl;
        
        // Run main loop
        try {
            game_ui.run();
            std::cout << "Main loop exited normally." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "ERROR in main loop: " << e.what() << std::endl;
            std::cerr.flush();
        } catch (...) {
            std::cerr << "Unknown error in main loop!" << std::endl;
            std::cerr.flush();
        }
        
        // Cleanup
        delete game_ui_ptr;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << std::endl;
        std::cerr << "========================================" << std::endl;
        std::cerr << "  FATAL ERROR" << std::endl;
        std::cerr << "========================================" << std::endl;
        std::cerr << "Exception caught: " << e.what() << std::endl;
        std::cerr << std::endl;
        std::cerr << "Please check:" << std::endl;
        std::cerr << "  1. SFML DLLs are in the same directory as the executable" << std::endl;
        std::cerr << "  2. Graphics drivers are up to date" << std::endl;
        std::cerr << "  3. All required files are present" << std::endl;
        std::cerr << std::endl;
        return 1;
    } catch (...) {
        std::cerr << std::endl;
        std::cerr << "========================================" << std::endl;
        std::cerr << "  FATAL ERROR" << std::endl;
        std::cerr << "========================================" << std::endl;
        std::cerr << "Unknown exception caught!" << std::endl;
        std::cerr << "This may indicate a memory corruption or system issue." << std::endl;
        std::cerr << std::endl;
        return 1;
    }
}

